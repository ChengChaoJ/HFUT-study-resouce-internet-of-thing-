#include "program_builder.hpp"

void LoongArch::ProgramBuilder::visit(ir::ir_reg &node) {}

void LoongArch::ProgramBuilder::visit(ir::ir_constant &node) {}

void LoongArch::ProgramBuilder::visit(ir::ir_basicblock &node) 
{
    for(auto & i : node.instructions)
        i->accept(*this);
}

void LoongArch::ProgramBuilder::visit(ir::ir_module &node) { 

    this->prog = std::make_shared<LoongArch::Program>();
    this->cur_mapping = std::make_shared<IrMapping>();

    node.reg_allocate();

    for(auto & [name,func] : node.usrfuncs){
        this->func_name = name;
        prog->functions.emplace_back(std::make_shared<LoongArch::Function>(name));
        func->accept(*this);
    }
    //后面可以用来处理全局变量，不过这里没有
}

void LoongArch::ProgramBuilder::visit(ir::ir_userfunc &node) {
    
    this->cur_func = prog->functions.back();
    
    this->cur_func->stack_size = 16; //默认为16字节，分别存储$ra和父函数的fp值

    for(auto [ir_reg,backend_reg] : node.regAllocateOut)
    {
        this->cur_mapping->reg_mapping[ir_reg->id] = Reg{backend_reg};
    }
    
    //获取所有func内部的变量
    for(auto & i : node.scope->ir_objs){
        this->cur_func->stack_size += i->get_size();
        this->cur_mapping->addrMemObj[i->get_addr()] = i;
        this->cur_mapping->revAddMemObj[i] = i->get_addr();
    }
    this->cur_func->stack_size = this->cur_func->bitalign(this->cur_func->stack_size, 16); //16字节对齐
    this->cur_func->cur_pos = this->cur_func->stack_size - 4;
    for(auto & i : node.scope->ir_objs){
        this->cur_mapping->objoffset_mapping[i] = this->cur_func->cur_pos;
        this->cur_func->cur_pos -= 4;
    }
    
    
    //build a entry
    cur_func->entry = std::make_shared<LoongArch::Block>(".entry_" + func_name);
    cur_func->blocks.push_back(cur_func->entry);
    prog->block_n++;
    //构建内部的block

    //添加第一批构造
    this->cur_func->entry->instructions.push_back(std::make_shared<LoongArch::RegImmInst>(RegImmInst::addi_d,Reg{sp},Reg{sp},-cur_func->stack_size));  //sp:栈顶指针
    this->cur_func->entry->instructions.push_back(std::make_shared<LoongArch::st>(Reg{ra},Reg{sp},cur_func->stack_size-8,st::st_d));//保存ra//返回地址
    this->cur_func->entry->instructions.push_back(std::make_shared<LoongArch::RegImmInst>(RegImmInst::addi_d,Reg{fp},Reg{sp},cur_func->stack_size)); //确定

    
    for(auto & block : node.bbs){
        auto cur_block_name = ".L" + std::to_string(prog->block_n);
        auto backend_block = std::make_shared<LoongArch::Block>(cur_block_name);
        //把entry的终点设定成L0
        if(cur_block_name == ".L1"){
            this->cur_func->entry->instructions.push_back(std::make_shared<LoongArch::Jump>(backend_block.get()));       
        }
        prog->block_n++;
        //确定映射关系
        this->cur_mapping->blockmapping[block] = backend_block;
        this->cur_mapping->rev_blockmapping[backend_block] = block;

        cur_func->blocks.push_back(backend_block);
    }

    //在遍历block的过程中，可能会牵扯到下一个block
    for(int i = 0; i < cur_func->blocks.size(); ++i){
        if(cur_func->blocks[i] != cur_func->entry){
            this->cur_block = cur_func->blocks[i];
            if(i < cur_func->blocks.size() - 1) this->next_block = cur_func->blocks[i+1];
            else this->next_block = nullptr;
            this->cur_mapping->rev_blockmapping[cur_func->blocks[i]]->accept(*this);
        }
    }

    // //构建phi指令
    struct PendingMove {
        std::shared_ptr<LoongArch::Block> block;
        LoongArch::Reg to, from;
    };

    std::vector<PendingMove> Pending_moves;
    for(auto &bb : node.bbs){
        for(auto &inst : bb->instructions){
            if(auto *cur = dynamic_cast<ir::phi*>(inst.get())){
                for(auto &prev : cur->uses){
                    auto b = cur_mapping->blockmapping[prev.second];
                    std::shared_ptr<ir::ir_reg> use_reg = std::dynamic_pointer_cast<ir::ir_reg>(prev.first);
                    if(use_reg) {
                        Reg temp = cur_mapping->transfer_reg(*use_reg.get());
                        Reg mid = cur_mapping->new_reg();
                        b->insert_before_jump(
                            std::make_shared<RegImmInst>(RegImmInst::addi_w,mid,temp,0)
                        );
                        Pending_moves.push_back({b,this->cur_mapping->transfer_reg(*cur->dst.get()),mid});
                    }else{
                        std::shared_ptr<ir::ir_constant> use_constant = std::dynamic_pointer_cast<ir::ir_constant>(prev.first);
                        //直接把那个立即数放到相应的phi的目的寄存器里面就行了，在uses block的的jump的前一句
                        //这里直接使用LoongArch的加载立即数指令
                        auto value = use_constant->init_val.value();
                        auto mid = cur_mapping->new_reg();
                        if(std::holds_alternative<int>(value)){
                            int value_num = std::get<int>(value);
                            b->insert_before_jump(
                                std::make_shared<LoongArch::LoadImm>(mid,value_num)
                            );
                        }
                        Pending_moves.push_back({b,this->cur_mapping->transfer_reg(*cur->dst.get()),mid});
                    }

                }
            }
        }
    }
    for(auto &i : Pending_moves){
        i.block->insert_before_jump(std::make_shared<RegImmInst>(RegImmInst::addi_w,i.to,i.from,0)); //插入一条move指令
        cur_func->regn = cur_mapping->regn;
    }
}

void LoongArch::ProgramBuilder::visit(ir::store &node) {
    auto addr = std::dynamic_pointer_cast<ir::ir_reg>(node.addr);
    auto irobj = this->cur_mapping->addrMemObj[addr];
    auto curpos = this->cur_mapping->objoffset_mapping[irobj];

    auto temp_reg = this->cur_mapping->new_reg();
    if(auto reg = std::dynamic_pointer_cast<ir::ir_reg>(node.value)){
        auto back_reg = this->cur_mapping->transfer_reg(*reg);
        this->cur_block->instructions.push_back(
            std::make_shared<LoongArch::st>(back_reg,Reg{fp},-curpos,st::st_w)
        );
    }else if (auto constant = std::dynamic_pointer_cast<ir::ir_constant>(node.value)){
        int value = std::get<int>(constant->init_val.value());
        this->cur_block->instructions.push_back(
            std::make_shared<LoongArch::RegImmInst>(RegImmInst::addi_w,temp_reg,Reg{zero},value)
        );
        this->cur_block->instructions.push_back(
            std::make_shared<LoongArch::st>(temp_reg,Reg{fp},-curpos,st::st_w)
        );
    }

}

void LoongArch::ProgramBuilder::visit(ir::jump &node) {
    auto target_block = cur_mapping->blockmapping[node.target];
    cur_block->instructions.push_back(
        std::make_shared<LoongArch::Jump>(target_block.get())
    );
}

void LoongArch::ProgramBuilder::visit(ir::br &node) {}

void LoongArch::ProgramBuilder::visit(ir::ret &node) {

    auto ret_reg = std::dynamic_pointer_cast<ir::ir_reg>(node.value);
    auto backend_reg = cur_mapping->transfer_reg(*ret_reg.get());
    cur_block->instructions.push_back(
        std::make_shared<LoongArch::RegRegInst>(RegRegInst::orw,Reg{4},backend_reg,Reg{0})
    );
    cur_block->instructions.push_back(
        std::make_shared<LoongArch::ld>(Reg{22},Reg{3},this->cur_func->stack_size - 8,ld::ld_d)
    );
    cur_block->instructions.push_back(
        std::make_shared<LoongArch::RegImmInst>(RegImmInst::addi_d,Reg{sp},Reg{sp},cur_func->stack_size)
    );
    cur_block->instructions.push_back(std::make_shared<LoongArch::jr>(true));

}

void LoongArch::ProgramBuilder::visit(ir::load &node) {
    auto addr = std::dynamic_pointer_cast<ir::ir_reg>(node.addr);
    auto irobj = this->cur_mapping->addrMemObj[addr];
    auto curpos = this->cur_mapping->objoffset_mapping[irobj];
    cur_block->instructions.push_back(
        std::make_shared<LoongArch::ld>(cur_mapping->transfer_reg(*node.dst),Reg{fp},-curpos,ld::ld_w)
    );
}

void LoongArch::ProgramBuilder::visit(ir::alloc &node) {

}

void LoongArch::ProgramBuilder::visit(ir::phi &node) {}

void LoongArch::ProgramBuilder::visit(ir::unary_op_ins &node) {}

void LoongArch::ProgramBuilder::visit(ir::binary_op_ins &node) {
    auto lhs = std::dynamic_pointer_cast<ir::ir_reg>(node.src1);
    auto rhs = std::dynamic_pointer_cast<ir::ir_reg>(node.src2);
    auto res = std::dynamic_pointer_cast<ir::ir_reg>(node.dst);

    auto backend_res = cur_mapping->transfer_reg(*res.get());

    RegRegInst::Type operation;
    RegImmInst::Type operation_imm;
    switch (node.op) {
        case binop::plus:
            operation = RegRegInst::add_w;
            operation_imm = RegImmInst::addi_w;
            break;
        case binop::divide:
            operation = RegRegInst::div_w;
            // operation_imm = RegImmInst::;
            break;
        case binop::minus:
            operation = RegRegInst::sub_w;
            break;
        case binop::multiply:
            operation = RegRegInst::mul_w;
            break;
        default:
            break;
        }

    if(rhs && lhs){
        auto backend_lhs = cur_mapping->transfer_reg(*lhs.get());
        auto backend_rhs = cur_mapping->transfer_reg(*rhs.get());

        cur_block->instructions.push_back(
            std::make_shared<LoongArch::RegRegInst>(operation,backend_res,backend_lhs,backend_rhs)
        );
    }else if(!rhs && !lhs){
        auto lhs_constant = std::dynamic_pointer_cast<ir::ir_constant>(node.src1);
        auto rhs_constant = std::dynamic_pointer_cast<ir::ir_constant>(node.src2);

        auto lhs_value = std::get<int>(lhs_constant->init_val.value());
        auto rhs_value = std::get<int>(rhs_constant->init_val.value());

        int res_value = 0;
        switch (node.op)
        {
        case binop::plus:
            res_value = lhs_value + rhs_value;
            break;
        case binop::minus:
            res_value = lhs_value - rhs_value;
            break;
        case binop::multiply:
            res_value = lhs_value * rhs_value;
            break;
        case binop::divide:
            res_value = lhs_value / rhs_value;
        default:
            break;
        }

        cur_block->instructions.push_back(
            std::make_shared<LoongArch::RegImmInst>(operation_imm,backend_res,Reg{0},res_value)
        ); 
    }else if(lhs && !rhs){
        auto rhs_constant = std::dynamic_pointer_cast<ir::ir_constant>(node.src2);
        auto rhs_value = std::get<int>(rhs_constant->init_val.value());
        auto backend_lhs = cur_mapping->transfer_reg(*lhs.get());        
        
        cur_block->instructions.push_back(std::make_shared<LoongArch::RegImmInst>(operation_imm,backend_res,backend_lhs,rhs_value));
    }else{
        auto lhs_constant = std::dynamic_pointer_cast<ir::ir_constant>(node.src1);
        auto lhs_value = std::get<int>(lhs_constant->init_val.value());
        auto backend_rhs = cur_mapping->transfer_reg(*rhs.get());        
        
        cur_block->instructions.push_back(std::make_shared<LoongArch::RegImmInst>(operation_imm,backend_res,backend_rhs,lhs_value));

    }

}

void LoongArch::ProgramBuilder::visit(ir::cmp_ins &node) {}

void LoongArch::ProgramBuilder::visit(ir::logic_ins &node) {}
