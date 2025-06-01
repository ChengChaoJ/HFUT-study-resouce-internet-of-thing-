#include "ir/irbuilder.hpp"

void ir::IrBuilder::visit(ast::compunit_syntax &node)
{
    /*
        init global variable
    */

    this->compunit = std::make_shared<ir::ir_module>();
    /*
        register lib functions
    */

    /*
        visit children
    */
    for(auto & i : node.global_defs)
        i->accept(*this);
}

void ir::IrBuilder::visit(ast::func_def_syntax &node){
    auto rettype = node.rettype;
    string name = node.name;
    
    this->cur_block = nullptr;
    this->cur_func = nullptr;
    this->cur_func_name = "";
    this->found_main = false;
    this->return_bb = nullptr;
    this->return_value.clear();
    /*
        first we should register this function in symbol table
    */
    cur_func = compunit->new_func(name);
    this->functions.push_func(name,cur_func);
    /*
        next we should deal with entry module
        in this module, we should insert instructions which is responsible for args.  
    */
    cur_func->entry = cur_func->new_block();
    cur_block = cur_func->entry;
    if(name == "main"){
        this->found_main = true;
    }
    return_bb = cur_func->new_block();
    /*
        deal with block
    */
    node.body->accept(*this);
    /*
        deal with ret
    */
    switch (rettype)
    {
    case vartype::VOID:
        {
            cur_block->push_back(std::make_shared<ir::jump>(return_bb));
            auto ret_value = std::make_shared<ir::ir_constant>(0);
            return_bb->push_back(std::make_shared<ir::ret>(ret_value,false));
            //false express non return
        }
        break;
    default:
        {
            auto ret_val = std::make_shared<ir::ir_constant>(0);
            return_value.emplace_back(ret_val,cur_block);
            cur_block->push_back(std::make_shared<ir::jump>(return_bb));
            /*
                set a default value to last block
            */
            /*
                add a phi inst
            */
            auto ret_dst = cur_func->new_reg(vartype::INT);
            auto phi_inst = std::make_shared<ir::phi>(ret_dst);
            phi_inst->uses = return_value;
            return_bb->push_back(phi_inst);
            return_bb->push_back(std::make_shared<ir::ret>(ret_dst,true));
        }
        break;
    }
}

void ir::IrBuilder::visit(ast::rel_cond_syntax &node)
{
    auto opt = node.op;
    Mode save_mode = mode;

    mode = Mode::normal;
    node.lhs->accept(*this);
    auto reg_lhs_val = std::dynamic_pointer_cast<ir::ir_value>(pass_value);
    
    node.rhs->accept(*this);
    auto reg_rhs_val = std::dynamic_pointer_cast<ir::ir_value>(pass_value);
    auto res_reg = cur_func->new_reg(vartype::INT);
    
    cur_block->push_back(std::make_shared<ir::cmp_ins>(res_reg,reg_lhs_val,reg_rhs_val,node.op));
    
    pass_value = res_reg;
    mode = save_mode;

}

void ir::IrBuilder::visit(ast::logic_cond_syntax &node)
{
    //op_and , op_or,需要处理短路，构建jumplist
    auto toCondJumpList = [&](){
        auto lhs_val = std::dynamic_pointer_cast<ir::ir_value>(pass_value);
        auto br = std::make_shared<ir::br>(lhs_val,nullptr,nullptr);
        cur_block->push_back(br);
        ptr<jumpList> jl = std::make_shared<jumpList>();
        jl->trueList.push_back(&br->target_true);
        jl->falseList.push_back(&br->target_false);
        return jl;
    };
    switch (node.op)
    {
    case relop::op_and :
    {
        if(mode == Mode::conditon){
            node.lhs->accept(*this);
            auto lhs_jl = toCondJumpList();
            cur_block = cur_func->new_block();
            for(auto & bb : lhs_jl->trueList){
                *bb = cur_block;
            }
            node.rhs->accept(*this);
            auto rhs_jl = toCondJumpList();
            for(auto & bb : lhs_jl->falseList){
                rhs_jl->falseList.push_back(bb);
            }
            pass_value = rhs_jl;
        }
    }    
    break;
    case relop::op_or :
    {
        if(mode == Mode::conditon){
            node.lhs->accept(*this);
            auto lhs_jl = toCondJumpList();
            cur_block = cur_func->new_block();
            for(auto & bb : lhs_jl->falseList){
                *bb = cur_block;
            }
            node.rhs->accept(*this);
            auto rhs_jl = toCondJumpList();
            for(auto & bb : lhs_jl->trueList){
                rhs_jl->trueList.push_back(bb);
            }
            pass_value = rhs_jl;
        }
    }
    default:
        break;
    }
}

void ir::IrBuilder::visit(ast::binop_expr_syntax &node)
{
    /*
        the output of binary expression is a value, not a condition
    */
    node.lhs->accept(*this);
    auto exp1=pass_value;
    node.rhs->accept(*this);
    auto exp2=pass_value;
    auto dst = cur_func->new_reg(vartype::INT);
    cur_block->push_back(std::make_shared<ir::binary_op_ins>(dst,exp1,exp2,node.op));
    this->pass_value=dst;
}

void ir::IrBuilder::visit(ast::unaryop_expr_syntax &node)
{
    node.rhs->accept(*this);
    auto exp =pass_value;
    switch (node.op)
    {
    case unaryop::plus:
        {
            pass_value = exp;
        }
        break;
    case unaryop::minus:
        {
            auto dst = cur_func->new_reg(vartype::INT);
            auto zero =std::make_shared<ir::ir_constant>(0);
            cur_block->push_back(std::make_shared<ir::binary_op_ins>(dst,zero,exp,binop::minus));
            this->pass_value = dst;
        }
    case unaryop::op_not:
        {
            auto dst = cur_func->new_reg(vartype::INT);
            auto zero = std::make_shared<ir_constant>(0);
            auto cmp = std::make_shared<ir::cmp_ins>(dst,zero,exp,relop::equal);
            auto true_block = cur_func->new_block();
            auto false_block = cur_func->new_block();
            cur_block->push_back(cmp);
            cur_block->push_back(std::make_shared<ir::br>(dst,true_block,false_block));
            auto end_block = cur_func->new_block();
            cur_block = end_block;
            true_block->push_back(std::make_shared<ir::jump>(end_block));
            false_block->push_back(std::make_shared<ir::jump>(end_block));
            auto phi_dst = cur_func->new_reg(vartype::INT);
            auto phi = std::make_shared<ir::phi>(phi_dst);
            auto one = std::make_shared<ir::ir_constant>(1);
            phi->uses.emplace_back(zero,false_block);
            phi->uses.emplace_back(one,true_block);
            end_block->push_back(phi);
            pass_value = phi_dst;
        }
    default:
        break;
    }

}

void ir::IrBuilder::visit(ast::lval_syntax &node)
{
    auto ret_reg = cur_func->new_reg(node.restype);
    auto name = node.name;
    /*
        we should find name in symbol table
        now laval is ident
        load right
    */
    auto obj = scope.find_var(name);
    auto addr = obj->addr;
    cur_block->push_back(std::make_shared<ir::load>(ret_reg,addr));
    /*
        pass obj to other module
    */
    // this->ir_value = obj;//ccx modified it
    this->pass_value=ret_reg;


}



void ir::IrBuilder::visit(ast::literal_syntax &node)
{
    auto constant = std::make_shared<ir::ir_constant>(node.intConst);
    pass_value = constant;
}

void ir::IrBuilder::visit(ast::var_def_stmt_syntax &node)
{
    //创建alloc指令
    auto var_name = node.name;
    auto obj = cur_func->new_obj(var_name);
    auto alloc_inst = std::make_shared<ir::alloc>(obj);
    //将指令放到BB中
    cur_block->push_back(std::static_pointer_cast<ir::ir_instr>(alloc_inst));
    //更新符号表中的内容
    this->scope.push_var(var_name,obj);

    if(node.initializer.get())//如果有初始值，进行赋值操作
    {
        node.initializer->accept(*this);
        Case(ir::ir_reg,value,pass_value){
            //取出变量对应的地址所在的寄存器
            auto target_obj=this->scope.find_var(var_name);
            auto addr=target_obj->get_addr();
            //利用store指令将需要的值放到目标的变量中
            auto assign_inst=std::make_shared<ir::store>(addr,value);
            cur_block->push_back(assign_inst);
        }
        else Case(ir::ir_constant,value,pass_value){
            //取出变量对应的地址所在的寄存器
            auto target_obj=this->scope.find_var(var_name);
            auto addr=target_obj->get_addr();
            //利用store指令将需要的值放到目标的变量中
            auto assign_inst=std::make_shared<ir::store>(addr,value);
            cur_block->push_back(assign_inst);
        }
        else{
            assert(0);//子函数传递类型不对
        }
    }
    

}

void ir::IrBuilder::visit(ast::assign_stmt_syntax &node)
{
    auto name=node.target->name;
    //查找对应的变量的地址
    auto obj=this->scope.find_var(name);
    auto addr=obj->get_addr();
    //获取value
    node.value->accept(*this);

    auto assign_inst=std::make_shared<ir::store>(addr,pass_value);
    this->cur_block->push_back(assign_inst);
   
}

void ir::IrBuilder::visit(ast::block_syntax &node)
{
    //由于我们的函数没有参数，所以，直接让block来处理scope的事情
    this->scope.enter();
    for(auto i :node.body)
    {
        i->accept(*this);
    }
    this->scope.exit();
}

void ir::IrBuilder::visit(ast::if_stmt_syntax &node)
{
    //首先访问cond得到保存有条件表达式结果的寄存器
    
    mode = Mode::conditon;
    node.pred->accept(*this);
    mode = Mode::normal;

    ptr<jumpList> jplst = std::make_shared<jumpList>();
    Case(ir::ir_reg,cond,pass_value)
    {
        //这句话是条件语句
        auto cond_inst=std::make_shared<ir::br>(cond,nullptr,nullptr);
        //在上一个block里面的最后一条加一个跳转，一开始是置空的，留在后面填写
        cur_block->push_back(cond_inst);
        jplst->trueList.push_back(&cond_inst->target_true);
        jplst->falseList.push_back(&cond_inst->target_false);
    }
    else
       jplst = std::dynamic_pointer_cast<ir::jumpList>(pass_value); 

    if(node.else_body)
    {
        //构建if和else的BB块
        ptr<ir::ir_basicblock> true_entry = cur_func->new_block();
        ptr<ir::ir_basicblock> false_entry = cur_func->new_block();
        ptr<ir::ir_basicblock> exit_entry = cur_func->new_block();
        cur_block = true_entry;
        node.then_body->accept(*this);
        cur_block->push_back(std::make_shared<ir::jump>(exit_entry));
        cur_block = false_entry;
        node.else_body->accept(*this);
        cur_block->push_back(std::make_shared<ir::jump>(exit_entry));
        //设置cond的true和false出口
        for(auto & i : jplst->trueList)
            *i = true_entry;
        for(auto & i : jplst->falseList)
            *i = false_entry;
        cur_block = exit_entry;
        //将设置if和else的BB的出口
    }
    else
    {
        //构建if和else的BB块
        ptr<ir::ir_basicblock> true_entry = cur_func->new_block();
        ptr<ir::ir_basicblock> exit_entry = cur_func->new_block();
        cur_block = true_entry;
        node.then_body->accept(*this);
        //将设置if和else的BB的出口
        cur_block->push_back(std::make_shared<ir::jump>(exit_entry));
        //设置cond的true和false出口
        for(auto & i : jplst->trueList)
            *i = true_entry;
        for(auto & i : jplst->falseList)
            *i = exit_entry;
        cur_block = exit_entry;
    }
    // assert(cond_inst->get_target_false()&&cond_inst->get_target_true());
}

void ir::IrBuilder::visit(ast::return_stmt_syntax &node)
{

    if(cur_func->rettype != vartype::VOID)//有返回值
    {
        node.exp->accept(*this);
        //放置好返回值和来源BB后跳转到return_bb
        return_value.emplace_back(pass_value,cur_block);
        cur_block->push_back(std::make_shared<ir::jump>(return_bb));
       
    }
    else//没有返回值
    {
        //直接跳转到return的BB
        auto jmp_inst = std::make_shared<ir::jump>(return_bb);
        cur_block->push_back(jmp_inst);

    }
    //对BB进行划分，这个BB用于防止return后面的部分（这部分永远不会被执行到）
    this->cur_block = this->cur_func->new_block();

}

void ir::IrBuilder::visit(ast::var_decl_stmt_syntax &node)
{
    for(auto i :node.var_def_list)
    {
        i->accept(*this);
    }
}
