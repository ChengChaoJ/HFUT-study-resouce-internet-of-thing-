#include "ir/ir.hpp"
#include <unordered_map>
#include <set>
#include "ir.hpp"
#include "loongarch/register_allocator.hpp"

void ir::ir_reg::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}
void ir::ir_reg::print(std::ostream & out ) { 
    out << "%r" << this->id << '\t';
}
void ir::ir_memobj::accept(ir_visitor &visitor) {

}
void ir::ir_memobj::print(std::ostream & out ) {
    out << this->name << " ";
    out << this->size << std::endl;
}
void ir::ir_scope::accept(ir_visitor &visitor) {

}
void ir::ir_scope::print(std::ostream & out )
{
}
void ir::ir_basicblock::push_back(ptr<ir_instr> inst)
{
    this->instructions.push_back(inst);
}

void ir::ir_basicblock::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::ir_basicblock::print(std::ostream & out )
{
}

std::shared_ptr<ir::ir_instr> ir::ir_basicblock::getLastInst() {
  return this->instructions.back();
}
std::shared_ptr<ir::ir_instr> ir::ir_basicblock::getFirstInst() {
  return this->instructions.front();
}
std::string ir::ir_basicblock::getName() { return this->name; }

void ir::ir_basicblock::for_each(std::function<void(std::shared_ptr<ir::ir_instr>)> f, bool isReverse) {
    if(!isReverse){
        for(auto & inst : this->instructions){
            f(inst);
        }
    }else{
        auto reverseInstruction(this->instructions);
        std::reverse(this->instructions.begin(),this->instructions.end());
        for(auto & inst : reverseInstruction){
            f(inst);
        }
        std::reverse(this->instructions.begin(),this->instructions.end());
    }
}

ptr<ir::ir_userfunc> ir::ir_module::new_func(std::string name) {
  auto pfunc = std::make_shared<ir_userfunc>(name);
  usrfuncs.push_back({name, pfunc});
  return pfunc;
}

void ir::ir_module::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::ir_module::print(std::ostream & out)
{
}

void ir::ir_module::reg_allocate() {
    for(auto & [name, func] : this->usrfuncs){
        func->reg_allocate();
    }
}

ir::ir_userfunc::ir_userfunc(std::string name) : ir_func(name) {
    this->scope = std::make_unique<ir::ir_scope>();
}

ptr<ir::ir_memobj> ir::ir_userfunc::new_obj(std::string name) {
  auto addr = this->new_reg(vartype::INTADDR);
  auto obj = std::make_shared<ir_memobj>(name, addr, i32_size);
  this->scope->ir_objs.push_back(obj);
  return obj;
}

ptr<ir::ir_reg> ir::ir_userfunc::new_reg(vartype type)
{
    int reg_size = 4;
    return std::make_shared<ir_reg>(max_reg++,type,reg_size);
}

ptr<ir::ir_basicblock> ir::ir_userfunc::new_block()
{
    auto bb = std::make_shared<ir_basicblock>(max_bb++);
    this->bbs.push_back(bb);
    return bb;
}

void ir::ir_userfunc::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::ir_userfunc::print(std::ostream & out)
{
}

std::unordered_map<ptr<ir::ir_value>,Pass::LiveInterval> ir::ir_userfunc::GetLiveInterval() 
{
    //正向遍历，求每一条指令行号的映射
    auto linerSequence = this->GetLinerSequence();
    int instNumber = 0;
    
    std::set<ptr<ir::ir_value>> phi_use;
    std::set<ptr<ir::ir_value>> phi_def;

    for(auto & bb : linerSequence){
        bb->for_each(
            [&](std::shared_ptr<ir::ir_instr> inst){
            this->instLineNumber[inst] = instNumber;
            this->revInstLineNumber[instNumber] = inst;
            instNumber += 2;

            if(auto insPhi = std::dynamic_pointer_cast<phi>(inst)){
                for(auto defreg : insPhi->def_reg()){
                    phi_def.insert(defreg);
                }
                for(auto & phiuse : insPhi->use_reg())
                    phi_use.insert(phiuse);
            }

        },false); 
    }
    std::unordered_map<ptr<ir::ir_value>,Pass::LiveInterval> liveInterval;
    //反向遍历，求LiveInterval
    std::reverse(linerSequence.begin(),linerSequence.end());
    for(auto & bb : linerSequence){
        bb->for_each([&](std::shared_ptr<ir::ir_instr> inst){
            for(auto inst_use : inst->use_reg()){
                if(phi_use.count(inst_use)){
                    liveInterval[inst_use].r = std::max(liveInterval[inst_use].r,this->instLineNumber[inst] + 1);
                }else{
                    liveInterval[inst_use].r = std::max(liveInterval[inst_use].r,this->instLineNumber[inst] + 1);
                }
            }
            for(auto inst_def : inst->def_reg()){
                if(inst_def && phi_def.count(inst_def)){
                    liveInterval[inst_def].l =  this->instLineNumber[bb->getFirstInst()]; 
                    liveInterval[inst_def].r = std::max(this->instLineNumber[bb->getLastInst()] + 2,liveInterval[inst_def].r);
                }else if(inst_def){
                    liveInterval[inst_def].l = 1 + this->instLineNumber[inst];
                }
            }
        },true);
    }

    return liveInterval;
}

std::vector<ptr<ir::ir_basicblock>> ir::ir_userfunc::GetLinerSequence() 
{
    std::vector<std::shared_ptr<ir::ir_basicblock>> linerSequence; 
    std::function<void(std::shared_ptr<ir::ir_basicblock>)> dfs = [&dfs,&linerSequence](std::shared_ptr<ir::ir_basicblock> bb)
    {
        linerSequence.push_back(bb);
        if(auto lastInst = std::dynamic_pointer_cast<ir::jump>(bb->getLastInst())){
            dfs(lastInst->getTarget());
        }else if(auto lastInst = std::dynamic_pointer_cast<ir::br>(bb->getLastInst())) {
            dfs(lastInst->get_target_true());
            dfs(lastInst->get_target_false());
        }
    };
    dfs(this->entry);
    return linerSequence;
}

void ir::ir_userfunc::reg_allocate() {
    auto reg_allocation = std::make_shared<LoongArch::ColoringAllocator>(shared_from_this());
    this->regAllocateOut = reg_allocation->run();
}

bool ir::ir_func::set_retype(vartype rettype)
{
    this->rettype = rettype; 
    return true;
}

void ir::store::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::store::print(std::ostream & out )
{
    out << "store" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
    out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::store::use_reg() {
    return {this->addr,this->value};
}

std::vector<ptr<ir::ir_value>> ir::store::def_reg() {
    return {};
}

void ir::jump::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::jump::print(std::ostream & out )
{
    out << "br-jump" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
    out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::jump::use_reg() {
  return {};
}

std::vector<ptr<ir::ir_value>> ir::jump::def_reg() {
  return {};
}

std::shared_ptr<ir::ir_basicblock> ir::jump::getTarget() {
  return this->target;
}

void ir::br::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::br::print(std::ostream & out )
{
    out << "br" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
        out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::br::use_reg() {
     return {this->cond};
}

std::vector<ptr<ir::ir_value>> ir::br::def_reg() { return std::vector<ptr<ir::ir_value>>(); }

void ir::ret::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::ret::print(std::ostream & out)
{
    out << "ret" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
        out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::ret::use_reg() { return {this->value};}

std::vector<ptr<ir::ir_value>> ir::ret::def_reg() { return std::vector<ptr<ir::ir_value>>(); }

void ir::load::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::load::print(std::ostream & out )
{
    out << "load" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
    out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::load::use_reg() {
    return {addr};
}

std::vector<ptr<ir::ir_value>> ir::load::def_reg() {
    return {dst};
}

void ir::alloc::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::alloc::print(std::ostream & out )
{
    out << "alloc" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
    out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::alloc::use_reg() {
  return std::vector<ptr<ir::ir_value>>();
}

std::vector<ptr<ir::ir_value>> ir::alloc::def_reg() {
  return {var->get_addr()};
}

void ir::phi::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::phi::print(std::ostream & out )
{
    out << "phi" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    
    }
        out << '\n';
}

std::vector<ptr<ir::ir_value>> ir::phi::use_reg() {
    std::vector<ptr<ir::ir_value>> use_regs;
    std::vector<ptr<ir::ir_value>> use_values;
    for(auto & [value, bb] : this->uses) {
        if(auto reg = std::dynamic_pointer_cast<ir_reg>(value)){
            use_regs.push_back(value);
        }else if(auto constant_value = std::dynamic_pointer_cast<ir_constant>(value)){
            use_values.push_back(value);
        }
    }
    return use_regs;
}

std::vector<ptr<ir::ir_value>> ir::phi::def_reg() { 
    return {this->dst}; 
}

void ir::unary_op_ins::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::unary_op_ins::print(std::ostream & out )
{
    
}

std::vector<ptr<ir::ir_value>> ir::unary_op_ins::use_reg() {
    return {this->src};
}

std::vector<ptr<ir::ir_value>> ir::unary_op_ins::def_reg() {
    return {this->dst};
};

void ir::binary_op_ins::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::binary_op_ins::print(std::ostream & out )
{
    out << "binary" << '\t';
    out << "def:" << '\t';
    auto defs = this->def_reg();
    for(auto & i : defs){
        i->print();
    }
    out << "use:" << '\t';
    auto uses = this->use_reg();
    for(auto & i : uses){
        i->print();
    }
}

std::vector<ptr<ir::ir_value>> ir::binary_op_ins::use_reg() {
  return {this->src1,this->src2};
}

std::vector<ptr<ir::ir_value>> ir::binary_op_ins::def_reg() {
  return {this->dst};
}

void ir::ir_constant::accept(ir_visitor &visitor) { visitor.visit(*this); }

void ir::ir_constant::print(std::ostream &out)
{
}

void ir::jumpList::accept(ir_visitor &visitor)
{
}

void ir::jumpList::print(std::ostream &out)
{
}

void ir::cmp_ins::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::cmp_ins::print(std::ostream &out)
{
}

std::vector<ptr<ir::ir_value>> ir::cmp_ins::use_reg() {
  return {this->src1,this->src2};
}

std::vector<ptr<ir::ir_value>> ir::cmp_ins::def_reg() {
  return {this->dst};
}

void ir::logic_ins::accept(ir_visitor &visitor)
{
    visitor.visit(*this);
}

void ir::logic_ins::print(std::ostream &out)
{
}

std::vector<ptr<ir::ir_value>> ir::logic_ins::use_reg() {
  return {this->src1,this->src2};
}

std::vector<ptr<ir::ir_value>> ir::logic_ins::def_reg() {
  return {this->dst};
}

std::vector<ptr<ir::ir_value>> ir::reg_write_ins::use_reg() {
  return std::vector<ptr<ir::ir_value>>();
}

std::vector<ptr<ir::ir_value>> ir::reg_write_ins::def_reg() {
  return std::vector<ptr<ir::ir_value>>();
}

std::vector<ptr<ir::ir_value>> ir::control_ins::use_reg() {
  return std::vector<ptr<ir::ir_value>>();
}

std::vector<ptr<ir::ir_value>> ir::control_ins::def_reg() {
  return std::vector<ptr<ir::ir_value>>();
}
