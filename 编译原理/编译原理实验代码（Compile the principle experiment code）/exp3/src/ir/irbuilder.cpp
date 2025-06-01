#include "irbuilder.hpp"

void ir::IrBuilder::visit(ast::compunit_syntax &node)
{
    /*
        init global variable
    */

    this->compunit = std::make_shared<ir::ir_module>();
    
    /*
        register lib functions
    */
    //


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
    node.lhs->accept(*this);
    auto exp1=pass_value;
    node.rhs->accept(*this);
    auto exp2=pass_value;
    auto dst = cur_func->new_reg(vartype::BOOL);
    cur_block->push_back(std::make_shared<ir::cmp_ins>(dst,exp1,exp2,node.op));
    this->pass_value=dst;
}

void ir::IrBuilder::visit(ast::logic_cond_syntax &node)
{
    node.lhs->accept(*this);
    auto exp1=pass_value;
    node.rhs->accept(*this);
    auto exp2=pass_value;
    auto dst = cur_func->new_reg(vartype::BOOL);
    cur_block->push_back(std::make_shared<ir::logic_ins>(dst,exp1,exp2,node.op));
    this->pass_value=dst;
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
    auto dst = cur_func->new_reg(node.restype);
    cur_block->push_back(std::make_shared<ir::binary_op_ins>(dst,exp1,exp2,node.op));
    this->pass_value=dst;
}

void ir::IrBuilder::visit(ast::unaryop_expr_syntax &node)
{
    node.rhs->accept(*this);
    auto exp=pass_value;
    auto dst = cur_func->new_reg(node.restype);
    cur_block->push_back(std::make_shared<ir::unary_op_ins>(dst,exp,node.op));
    this->pass_value=dst;
}

void ir::IrBuilder::visit(ast::lval_syntax &node)
{
    auto newobj = this->scope.find_var(node.name);//符号表中查找
    if (newobj)//找到了，生成load指令来获取左值的当前值
    {
        auto dst = cur_func->new_reg(node.restype);
        cur_block->push_back(std::make_shared<ir::load>(dst, newobj->get_addr()));
        this->pass_value = dst;
    }

}

void ir::IrBuilder::visit(ast::literal_syntax &node)
{
    auto constant = std::make_shared<ir::ir_constant>(node.intConst);
    pass_value = constant;
}

void ir::IrBuilder::visit(ast::var_def_stmt_syntax &node)
{
    // 创建一个新的IR对象（内存空间）
    auto newobj = cur_func->new_obj(node.name);
    //分配内存
    cur_block->push_back(std::make_shared<ir::alloc>(newobj));
    // 添加到当前函数作用域的符号表
    this->scope.push_var(node.name, newobj);
    // 检查是否有初始化表达式
    if (node.initializer) 
    {   // 如果有初始化表达式，存储(store)值
        node.initializer->accept(*this); // 生成初始化表达式的IR 计算后的值存储在pass_value中

        // 创建一个IR指令，将初始化表达式的结果赋值给新创建的内存空间
        cur_block->push_back(std::make_shared<ir::store>(newobj->get_addr(), pass_value));
    }
}

void ir::IrBuilder::visit(ast::assign_stmt_syntax &node)
{
    //获取目标变量
    auto target = this->scope.find_var(node.target->name);
    //赋值表达式处理
    node.value->accept(*this);
    //赋值
    cur_block->push_back(std::make_shared<ir::store>(target->get_addr(), this->pass_value));
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
    //获取判断条件结果
    node.pred->accept(*this);
    auto cond_val = this->pass_value;

    //创建一个基本块来表示条件判断
    auto cond_block = cur_func->new_block();


    //处理分支
    auto then_block = cur_func->new_block();
    // auto else_block = node.else_body ? cur_func->new_block() : nullptr; 
    auto else_block = cur_func->new_block();
    auto after_if_bb = cur_func->new_block();

    // if (else_block) 
    // {
    //     cur_block->push_back(std::make_shared<ir::br>(cond_val, then_block, else_block));
    // } 
    // else 
    // {
    //     cur_block->push_back(std::make_shared<ir::br>(cond_val, then_block, after_if_bb));
    // }
    cur_block->push_back(std::make_shared<ir::br>(cond_val, then_block, else_block));

    // 更新当前基本块为then块，并处理then分支中的语句
    cur_block = then_block;
    node.then_body->accept(*this);

    // if (else_block) 
    // {
    //     cur_block->push_back(std::make_shared<ir::jump>(after_if_bb));
    // }
    cur_block->push_back(std::make_shared<ir::jump>(after_if_bb));

    // //处理else语句
    // if (node.else_body) 
    // {
    //     cur_block = else_block;
    //     node.else_body->accept(*this);
    //     // 处理else分支后的控制流
    //     cur_block->push_back(std::make_shared<ir::jump>(after_if_bb));
    // }
    cur_block = else_block;
    if (node.else_body) {
        node.else_body->accept(*this);
    } else {
        // 如果没有else_body，至少生成一个NOP或者一个不影响程序的语句
        // 例如，我们可以生成一个对常量的赋值，这个值永远不会被使用
        cur_block->push_back(std::make_shared<ir::nop>()); // 或者其他不影响程序的指令
    }

    // else块结束后，无条件跳转到after_if_bb
    cur_block->push_back(std::make_shared<ir::jump>(after_if_bb));

    //更新当前基本块
    cur_block = after_if_bb;
}



void ir::IrBuilder::visit(ast::return_stmt_syntax &node)
{

    if(cur_func->rettype != vartype::VOID)//有返回值
    {
        node.exp->accept(*this);//计算表达式的值,存入pass_value
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
