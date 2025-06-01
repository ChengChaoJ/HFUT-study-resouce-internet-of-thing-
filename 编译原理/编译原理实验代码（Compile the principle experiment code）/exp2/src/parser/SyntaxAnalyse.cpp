#include "SyntaxAnalyse.hpp"
#include "cstring"

extern ast::SyntaxTree syntax_tree;
ptr_list<ast::var_def_stmt_syntax> vartable;    //已定义的变量

void SyntaxAnalyseCompUnit(ast::compunit_syntax * &self, ast::compunit_syntax *compunit, ast::func_def_syntax *func_def)
{
    if(compunit){
        self = new ast::compunit_syntax;
        for(auto  i : compunit->global_defs){
            self->global_defs.emplace_back(std::shared_ptr<ast::func_def_syntax>(i));
        }//遍历，将compunit的所有global_defs加入self的global_defs中，意思就是用一个总的compunit存所有的compunit
        self->global_defs.emplace_back(func_def);//再把最后单个的函数加入到self->global_defs
    }else{
        self = new ast::compunit_syntax;
        self->global_defs.emplace_back(func_def);
    }
    syntax_tree.root = self;
}

void SyntaxAnalyseFuncDef(ast::func_def_syntax * &self, vartype var_type, char *Ident, ast::block_syntax *block)
{
    self = new ast::func_def_syntax;
    self->rettype = var_type;   //类型
    self->name = Ident;         //名字
    self->body = std::shared_ptr<ast::block_syntax>(block);//指向block的指针
}

void SynataxAnalyseFuncType(vartype &self, char* type)
{
    self = ( !strcmp(type,"int") ? vartype::INT : vartype::VOID);//可以扩充
}

void SynataxAnalyseBlock(ast::block_syntax *&self, ast::block_syntax *block_items)
{
    self = new ast::block_syntax;
    if(block_items){
        for(auto  i : block_items->body){
            self->body.emplace_back(i);
        }
    }
}    

void SynataxAnalyseBlockItems(ast::block_syntax *&self, ast::block_syntax *block_items, ast::stmt_syntax *stmt)
{
    self = new ast::block_syntax;
    if(block_items && stmt){//都不为空指针
        for(auto  i : block_items->body){
            self->body.emplace_back(i);//是用一个总的block_items存所有的block_items
        }
        self->body.emplace_back(stmt);
    }else if(!stmt && !block_items){//都为空
        self = nullptr;
    }else {//stem不为空
        self->body.emplace_back(stmt);
    }
}

void SynataxAnalyseStmtReturn(ast::stmt_syntax *&self, ast::expr_syntax *exp)
{
    auto syntax = new ast::return_stmt_syntax;
    if(exp)
        syntax->exp = std::shared_ptr<ast::expr_syntax>(exp);
    else
        syntax->exp = 0;
    self = static_cast<ast::stmt_syntax*>(syntax);//将syntax转为stmt_syntax类型的self
}

void SynataxAnalysePrimaryExpIntConst(ast::expr_syntax *&self, char *current_symbol)
{
    auto syntax = new ast::literal_syntax;
    syntax->intConst = std::stoi(current_symbol);//stoi函数将字符串转为数字 如“123”->123
    self = static_cast<ast::expr_syntax*>(syntax);
}
//a-难度
void SynataxAnalyseStmtBlock(ast::stmt_syntax *&self, ast::block_syntax *block)
{
    auto syntax = new ast::block_syntax;
    if (block)
    {
        for(auto i : block->body)
        {
            syntax->body.emplace_back(i);
        }
    }
    self = static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalysePrimaryExpVar(ast::expr_syntax* &self, char* current_symbol)
{
    for (auto it : vartable)
    {
        if (it->name == current_symbol)//变量已经定义
        {
            auto syntax = new ast::lval_syntax;
            syntax->restype = it->restype;
            syntax->name = current_symbol;

            self = static_cast<ast::expr_syntax*>(syntax);
            return;
        }
    }  
    throw __EXCEPTION__; 
}


void SynataxAnalyseVarDecl(ast::stmt_syntax *&self, ast::var_def_stmt_syntax *var_def, ast::var_decl_stmt_syntax *var_def_group)
{
    auto syntax = new ast::var_decl_stmt_syntax;
    if (var_def)
    {
        syntax->var_def_list.emplace_back(std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
        vartable.emplace_back(std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
    }
    if (var_def_group)
    {
        for (auto i : var_def_group->var_def_list)
        {
            syntax->var_def_list.emplace_back(std::shared_ptr<ast::var_def_stmt_syntax>(i));
            vartable.emplace_back(std::shared_ptr<ast::var_def_stmt_syntax>(i));
        }
    }
    self = static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalyseVarDefGroup(ast::var_decl_stmt_syntax *&self, ast::var_def_stmt_syntax *var_def, ast::var_decl_stmt_syntax *var_def_group)
{
    self = new ast::var_decl_stmt_syntax;
    if (var_def)
    {
        self->var_def_list.emplace_back(std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
    }

    if (var_def_group)
    {
        for (auto i : var_def_group->var_def_list)
        {
            self->var_def_list.emplace_back(std::shared_ptr<ast::var_def_stmt_syntax>(i));
        }
    }
}

void SynataxAnalyseVarDef(ast::var_def_stmt_syntax *&self, char *ident, ast::expr_syntax *init)
{
    for (auto it : vartable)
    {
        if (!strcmp(it->name.c_str(), ident))//出现重名
        {
            throw __EXCEPTION__;
            return;
        }
    }
    self = new ast::var_def_stmt_syntax;
    self->restype = vartype::INT;
    self->name = ident;
    if (init)
    {
        self->haveInit = true;
        self->initializer = std::shared_ptr<ast::expr_syntax>(init);

        auto binop_exp = dynamic_cast<ast::binop_expr_syntax*>(init);
        auto unaryop_exp = dynamic_cast<ast::unaryop_expr_syntax*>(init);
        auto literal_exp = dynamic_cast<ast::literal_syntax*>(init);

        if (binop_exp != nullptr)  
        {
            self->value = binop_exp->intConst;
        }
        else if (unaryop_exp != nullptr)  
        {
            self->value = unaryop_exp->intConst;
        }
        else if (literal_exp != nullptr)   
        {
            self->value = literal_exp->intConst;
        }
    }
}

void SynataxAnalyseAddExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    auto syntax = new ast::binop_expr_syntax;
    syntax->lhs = std::shared_ptr<ast::expr_syntax>(exp1);
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(exp2);

    auto binop_exp1 = dynamic_cast<ast::binop_expr_syntax*>(exp1);
    auto unaryop_exp1 = dynamic_cast<ast::unaryop_expr_syntax*>(exp1);
    auto literal_exp1 = dynamic_cast<ast::literal_syntax*>(exp1);
    auto lval_exp1 = dynamic_cast<ast::lval_syntax*>(exp1);
    int num1;
    auto binop_exp2 = dynamic_cast<ast::binop_expr_syntax*>(exp2);
    auto unaryop_exp2 = dynamic_cast<ast::unaryop_expr_syntax*>(exp2);
    auto literal_exp2 = dynamic_cast<ast::literal_syntax*>(exp2);
    auto lval_exp2 = dynamic_cast<ast::lval_syntax*>(exp2);
    int num2;

    if (binop_exp1 != nullptr)  //exp1为binop_expr_syntax
    {
        syntax->restype = binop_exp1->restype;
        num1 = binop_exp1->intConst;
    }
    else if (unaryop_exp1 != nullptr)   //exp1为unaryop_expr_syntax
    {
        syntax->restype = unaryop_exp1->restype;
        num1 = unaryop_exp1->intConst;
    }
    else if (literal_exp1 != nullptr)   //exp1为literal_syntax
    {
        syntax->restype = literal_exp1->restype;
        num1 = literal_exp1->intConst;
    }
    else if (lval_exp1 != nullptr)
    {
        syntax->restype = lval_exp1->restype;
        bool notFind = true;
        for (auto it : vartable)
        {
            if (it->name == lval_exp1->name)//变量已经定义
            {
                notFind = false;
                num1 = it->value;
            }
        }
        if (notFind)
            throw __EXCEPTION__;
    }

    if (binop_exp2 != nullptr)      
    {
        syntax->restype = binop_exp2->restype;
        num2 = binop_exp2->intConst;
    }
    else if (unaryop_exp2 != nullptr)
    {
        syntax->restype = unaryop_exp2->restype;
        num2 = unaryop_exp2->intConst;
    }
    else if (literal_exp2 != nullptr)
    {
        syntax->restype = literal_exp2->restype;
        num2 = literal_exp2->intConst;
    }
    else if (lval_exp2 != nullptr)
    {
        syntax->restype = lval_exp2->restype;
        bool notFind = true;
        for (auto it : vartable)
        {
            if (it->name == lval_exp2->name)//变量已经定义
            {
                notFind = false;
                num2 = it->value;
            }
        }
        if (notFind)
            throw __EXCEPTION__;
    }

    if (!strcmp(op, "+"))   //op为+//但是无法解决表达式是变量的问题
    {
        syntax->op = binop::plus;
        syntax->intConst = num1 + num2;
    }
    else if (!strcmp(op, "-"))
    {
        syntax->op = binop::minus;
        syntax->intConst = num1 - num2;
    }
    // std::cout << "*************************\n";
    // std::cout << syntax->intConst << "=" << num1 << op << num2 << std::endl;
    // std::cout << "*************************\n";

    self = static_cast<ast::expr_syntax*>(syntax);
}

//a难度
void SynataxAnalyseMulExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    auto syntax = new ast::binop_expr_syntax;
    syntax->lhs = std::shared_ptr<ast::expr_syntax>(exp1);
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(exp2);

    auto binop_exp1 = dynamic_cast<ast::binop_expr_syntax*>(exp1);
    auto unaryop_exp1 = dynamic_cast<ast::unaryop_expr_syntax*>(exp1);
    auto literal_exp1 = dynamic_cast<ast::literal_syntax*>(exp1);
    auto lval_exp1 = dynamic_cast<ast::lval_syntax*>(exp1);
    int num1;
    auto binop_exp2 = dynamic_cast<ast::binop_expr_syntax*>(exp2);
    auto unaryop_exp2 = dynamic_cast<ast::unaryop_expr_syntax*>(exp2);
    auto literal_exp2 = dynamic_cast<ast::literal_syntax*>(exp2);
    auto lval_exp2 = dynamic_cast<ast::lval_syntax*>(exp2);
    int num2;

    if (binop_exp1 != nullptr)
    {
        syntax->restype = binop_exp1->restype;
        num1 = binop_exp1->intConst;
    }
    else if (unaryop_exp1 != nullptr)
    {
        syntax->restype = unaryop_exp1->restype;
        num1 = unaryop_exp1->intConst;
    }
    else if (literal_exp1 != nullptr)
    {
        syntax->restype = literal_exp1->restype;
        num1 = literal_exp1->intConst;
    }
    else if (lval_exp1 != nullptr)
    {
        syntax->restype = lval_exp1->restype;
        bool notFind = true;
        for (auto it : vartable)
        {
            if (it->name == lval_exp1->name)//变量已经定义
            {
                notFind = false;
                num1 = it->value;
            }
        }
        if (notFind)
            throw __EXCEPTION__;
    }

    if (binop_exp2 != nullptr)      
    {
        syntax->restype = binop_exp2->restype;
        num2 = binop_exp2->intConst;
    }
    else if (unaryop_exp2 != nullptr)
    {
        syntax->restype = unaryop_exp2->restype;
        num2 = unaryop_exp2->intConst;
    }
    else if (literal_exp2 != nullptr)
    {
        syntax->restype = literal_exp2->restype;
        num2 = literal_exp2->intConst;
    }
    else if (lval_exp2 != nullptr)
    {
        syntax->restype = lval_exp2->restype;
        bool notFind = true;
        for (auto it : vartable)
        {
            if (it->name == lval_exp2->name)//变量已经定义
            {
                notFind = false;
                num2 = it->value;
            }
        }
        if (notFind)
            throw __EXCEPTION__;
    }

    if (!strcmp(op, "*"))   //op为*
    {
        syntax->op = binop::multiply;
        syntax->intConst = num1 * num2;
    }
    else if (!strcmp(op, "/"))
    {
        syntax->op = binop::divide;
        syntax->intConst = num1 / num2;    
    }
    // std::cout << "*************************\n";
    // std::cout << syntax->intConst << "=" << num1 << op << num2 << std::endl;
    // std::cout << "*************************\n";
    self = static_cast<ast::expr_syntax*>(syntax);
}

void SynataxAnalyseStmtAssign(ast::stmt_syntax *&self, ast::lval_syntax *target, ast::expr_syntax *value)
{    
    for (auto it : vartable)
    {
        if (it->name == target->name)//找到已定义的变量
        {
            auto syntax = new ast::assign_stmt_syntax;
            syntax->target = std::shared_ptr<ast::lval_syntax>(target);
            syntax->value = std::shared_ptr<ast::expr_syntax>(value);
            
            auto binop_exp = dynamic_cast<ast::binop_expr_syntax*>(value);
            auto unaryop_exp = dynamic_cast<ast::unaryop_expr_syntax*>(value);
            auto literal_exp = dynamic_cast<ast::literal_syntax*>(value);

            if (binop_exp != nullptr)  
            {
                it->value = binop_exp->intConst;
            }
            else if (unaryop_exp != nullptr)  
            {
                it->value = unaryop_exp->intConst;
            }
            else if (literal_exp != nullptr)   
            {
                it->value = literal_exp->intConst;
            }

                self = static_cast<ast::stmt_syntax*>(syntax);
                return;
        }
    }    
    throw __EXCEPTION__;
}

void SynataxAnalyseLval(ast::lval_syntax *&self, char *ident)
{
    for (auto it : vartable)
    {
        if (it->name == ident)//变量已经定义
        {
            self = new ast::lval_syntax;
            self->name = ident;
            self->restype = it->restype;
            return;
        }
    }  
    throw __EXCEPTION__;
}
//a+难度
void SynataxAnalyseStmtIf(ast::stmt_syntax *&self, ast::expr_syntax *cond, ast::stmt_syntax *then_body, ast::stmt_syntax *else_body)
{
    auto syntax = new ast::if_stmt_syntax;
    syntax->pred = std::shared_ptr<ast::expr_syntax>(cond);
    syntax->then_body = std::shared_ptr<ast::stmt_syntax>(then_body);
    if (else_body)
    {
        syntax->else_body = std::shared_ptr<ast::stmt_syntax>(else_body);
    }
    self = static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalyseLOrExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, ast::expr_syntax *cond2)
{
    auto syntax = new ast::logic_cond_syntax;
    syntax->lhs = std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(cond2);
    syntax->op = relop::op_or;
    self = static_cast<ast::expr_syntax*>(syntax);
}

void SynataxAnalyseLAndExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, ast::expr_syntax *cond2)
{
    auto syntax = new ast::logic_cond_syntax;
    syntax->lhs = std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(cond2);
    syntax->op = relop::op_and;
    self = static_cast<ast::expr_syntax*>(syntax);
}

void SynataxAnalyseEqExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, char *op, ast::expr_syntax *cond2)
{
    auto syntax = new ast::rel_cond_syntax;
    syntax->lhs = std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(cond2);
    
    //rel_cond_syntax类有值时使用
    // auto relop_exp1 = dynamic_cast<ast::rel_cond_syntax*>(cond1);
    // auto binop_exp1 = dynamic_cast<ast::binop_expr_syntax*>(cond1);
    // auto unaryop_exp1 = dynamic_cast<ast::unaryop_expr_syntax*>(cond1);
    // auto literal_exp1 = dynamic_cast<ast::literal_syntax*>(cond1);
    // auto lval_exp1 = dynamic_cast<ast::lval_syntax*>(cond1);
    // int num1;
    // auto binop_exp2 = dynamic_cast<ast::binop_expr_syntax*>(cond2);
    // auto unaryop_exp2 = dynamic_cast<ast::unaryop_expr_syntax*>(cond2);
    // auto literal_exp2 = dynamic_cast<ast::literal_syntax*>(cond2);
    // auto lval_exp2 = dynamic_cast<ast::lval_syntax*>(cond2);
    // int num2;

    // if (relop_exp1 != nullptr)
    // {
    //     // num1 = relop_exp1->intConst;
    // }
    // else if (binop_exp1 != nullptr)
    // {
    //     num1 = binop_exp1->intConst;
    // }
    // else if (unaryop_exp1 != nullptr)
    // {
    //     num1 = unaryop_exp1->intConst;
    // }
    // else if (literal_exp1 != nullptr)
    // {
    //     num1 = literal_exp1->intConst;
    // }
    // else if (lval_exp1 != nullptr)
    // {
    //     bool notFind = true;
    //     for (auto it : vartable)
    //     {
    //         if (it->name == lval_exp1->name)//变量已经定义
    //         {
    //             notFind = false;
    //             num1 = it->value;
    //         }
    //     }
    //     if (notFind)
    //         throw __EXCEPTION__;
    // }

    // if (binop_exp2 != nullptr)      
    // {
    //     num2 = binop_exp2->intConst;
    // }
    // else if (unaryop_exp2 != nullptr)
    // {
    //     num2 = unaryop_exp2->intConst;
    // }
    // else if (literal_exp2 != nullptr)
    // {
    //     num2 = literal_exp2->intConst;
    // }
    // else if (lval_exp2 != nullptr)
    // {
    //     bool notFind = true;
    //     for (auto it : vartable)
    //     {
    //         if (it->name == lval_exp2->name)//变量已经定义
    //         {
    //             notFind = false;
    //             num2 = it->value;
    //         }
    //     }
    //     if (notFind)
    //         throw __EXCEPTION__;
    // }

    if (!strcmp(op, "=="))
    {
        syntax->op = relop::equal;
    }
    else if (!strcmp(op, "!="))
    {
        syntax->op = relop::non_equal;
    }

    self = static_cast<ast::expr_syntax*>(syntax);
}


void SynataxAnalyseRelExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, char *op, ast::expr_syntax *exp)
{
    auto syntax = new ast::rel_cond_syntax;
    syntax->lhs = std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(exp);

    if (!strcmp(op, "<"))   //op为
    {
        syntax->op = relop::less;
    }
    else if (!strcmp(op, ">"))
    {
        syntax->op = relop::greater;   
    }
    else if (!strcmp(op, "<="))
    {
        syntax->op = relop::less_equal;  
    }
    else if (!strcmp(op, ">="))
    {
        syntax->op = relop::greater_equal;   
    }
    self = static_cast<ast::expr_syntax*>(syntax);
}

void SynataxAnalyseUnaryExp(ast::expr_syntax *&self, char *op, ast::expr_syntax *exp)
{//另一种写法，num1类型可变
    auto syntax = new ast::unaryop_expr_syntax;
    syntax->rhs = std::shared_ptr<ast::expr_syntax>(exp);

    ast::binop_expr_syntax* binop_expr = dynamic_cast<ast::binop_expr_syntax*>(exp);
    ast::literal_syntax* literal_exp = dynamic_cast<ast::literal_syntax*>(exp);
    ast::unaryop_expr_syntax* unaryop_exp = dynamic_cast<ast::unaryop_expr_syntax*>(exp);
    ast::lval_syntax* lval_exp = dynamic_cast<ast::lval_syntax*>(exp);

    if (binop_expr != nullptr) //exp为binop_expr_syntax
    {
        syntax->restype = binop_expr->restype;
        if (!strcmp(op, "+"))   
        {
            syntax->op = unaryop::plus;
            syntax->intConst = +(binop_expr->intConst);
        }
        else if (!strcmp(op, "-"))
        {
            syntax->op = unaryop::minus;
            syntax->intConst = -(binop_expr->intConst);
            // std::cout << "*************************\n";
            // std::cout << syntax->intConst << "=" << op << binop_expr->intConst << std::endl;
            // std::cout << "*************************\n";
        }
        else if (!strcmp(op, "!"))  //op为!
        {
            syntax->op = unaryop::op_not;
            syntax->intConst = !(literal_exp->intConst);
        }
        self = static_cast<ast::expr_syntax*>(syntax);
        return;
    }
    if (literal_exp != nullptr) //exp为literal_syntax
    {
        syntax->restype = literal_exp->restype;
        if (!strcmp(op, "+"))   
        {
            syntax->op = unaryop::plus;
            syntax->intConst = +(literal_exp->intConst);
        }
        else if (!strcmp(op, "-"))
        {
            syntax->op = unaryop::minus;
            syntax->intConst = -(literal_exp->intConst);
            // std::cout << "*************************\n";
            // std::cout << syntax->intConst << "=" << op << literal_exp->intConst << std::endl;
            // std::cout << "*************************\n";
        }
        else if (!strcmp(op, "!"))  //op为!
        {
            syntax->op = unaryop::op_not;
            syntax->intConst = !(literal_exp->intConst);
        }
        self = static_cast<ast::expr_syntax*>(syntax);
        return;
    }
    if (unaryop_exp != nullptr) //exp为unaryop_expr_syntax
    {
        syntax->restype = unaryop_exp->restype;
        if (!strcmp(op, "+"))   
        {
            syntax->op = unaryop::plus;
            syntax->intConst = +(unaryop_exp->intConst);
        }
        else if (!strcmp(op, "-"))
        {
            syntax->op = unaryop::minus;
            syntax->intConst = -(unaryop_exp->intConst);
            // std::cout << "*************************\n";
            // std::cout << syntax->intConst << "=" << op << unaryop_exp->intConst << std::endl;
            // std::cout << "*************************\n";
        }
        else if (!strcmp(op, "!"))  //op为!
        {
            syntax->op = unaryop::op_not;
            syntax->intConst = !(unaryop_exp->intConst);
        }
        self = static_cast<ast::expr_syntax*>(syntax);
        return;
    }

    if (lval_exp != nullptr) //exp为lval_syntax
    {
        syntax->restype = lval_exp->restype;
        for (auto it : vartable)
        {
            if (it->name == lval_exp->name)//变量已经定义
            {
                if (!strcmp(op, "+"))   
                {
                    syntax->op = unaryop::plus;
                    syntax->intConst = +it->value;
                }
                else if (!strcmp(op, "-"))
                {
                    syntax->op = unaryop::minus;
                    syntax->intConst = -it->value;
                    // std::cout << "*************************\n";
                    // std::cout << syntax->intConst << "=" << op << it->value << std::endl;
                    // std::cout << "*************************\n";
                }
                else if (!strcmp(op, "!"))  //op为!
                {
                    syntax->op = unaryop::op_not;
                    syntax->intConst = !it->value;
                }
                self = static_cast<ast::expr_syntax*>(syntax);
                return;
            }
        }
        throw __EXCEPTION__;
    }
}