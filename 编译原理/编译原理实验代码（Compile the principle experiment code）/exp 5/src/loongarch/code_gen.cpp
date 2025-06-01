#include "loongarch/code_gen.hpp"
#include "loongarch/inst.hpp"
#include "code_gen.hpp"

LoongArch::Program::Program() {
    this->block_n = 0;
}

void LoongArch::Program::get_asm(std::ostream& out)
{
    //可以首先处理一些全局变量的问题
    out << ".text" << '\n';      //标记代码段
    out << ".globl" << ' ' << "main" << '\n';    //标记main函数全局可见
    
    for(auto &func : functions){
        func->get_asm(out);
    }
}

LoongArch::Function::Function(std::string name): regn(0), stack_size(0) {
    this->name = name;
}

void LoongArch::Function::get_asm(std::ostream& out){
    out << this->name << " :" << '\n';
    for(auto &block : this->blocks)
        block->get_asm(out);
}

int LoongArch::Function::bitalign(int value, int align) { 
    return (value + align - 1) & ~(align - 1);
}

LoongArch::Block::Block(std::string name) {
    this->name = name;
}

void LoongArch::Block::insert_before_jump(std::shared_ptr<Inst> inst) {
    auto i = std::end(instructions);
    while(i != instructions.begin()){
        auto prev_i = std::prev(i);
        if((*prev_i)->as<LoongArch::Jump>() || (*prev_i)->as<LoongArch::jr>()) {
            i = prev_i; 
        }else{
            break;
        }
    }
    this->instructions.insert(i,inst);
}

void LoongArch::Block::get_asm(std::ostream &out) {
    out << this->name << ':' << '\n';
    for(auto inst : this->instructions){
        out << '\t';
        inst->gen_asm(out);
    }
}

LoongArch::Reg LoongArch::IrMapping::new_reg() { return Reg{regn++}; }

LoongArch::Reg LoongArch::IrMapping::transfer_reg(ir::ir_reg irReg) {
    auto it = reg_mapping.find(irReg.id);
    if(it != reg_mapping.end()) return it->second;
    Reg ret = new_reg();
    reg_mapping[irReg.id] = ret;
    return ret;
}


LoongArch::IrMapping::IrMapping() : regn(12) {}