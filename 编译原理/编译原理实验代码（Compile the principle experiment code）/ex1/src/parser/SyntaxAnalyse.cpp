#include "SyntaxAnalyse.hpp"
#include "cstring"

extern ast::SyntaxTree syntax_tree;
void SyntaxAnalyseCompUnit(ast::compunit_syntax * &self, ast::compunit_syntax *compunit, ast::func_def_syntax *func_def)
{
    if(compunit){
        self = new ast::compunit_syntax;
        for(auto  i : compunit->global_defs){
            self->global_defs.emplace_back(std::shared_ptr<ast::func_def_syntax>(i));
        }
        self->global_defs.emplace_back(func_def);
    }else{
        self = new ast::compunit_syntax;
        self->global_defs.emplace_back(func_def);
    }
    syntax_tree.root = self;
}

void SyntaxAnalyseFuncDef(ast::func_def_syntax * &self, vartype var_type, char *Ident, ast::block_syntax *block)
{
    self = new ast::func_def_syntax;
    self->name = Ident;
    self->rettype = var_type;
    self->body = std::shared_ptr<ast::block_syntax>(block);
}

void SynataxAnalyseFuncType(vartype &self, char* type)
{
    self = ( !strcmp(type,"int") ? vartype::INT : vartype::VOID);
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
    if(block_items && stmt){
        for(auto  i : block_items->body){
            self->body.emplace_back(i);
        }
        self->body.emplace_back(stmt);
    }else if(!stmt && !block_items){
        self = nullptr;
    }else {
        self->body.emplace_back(stmt);
    }
}

void SynataxAnalyseStmtReturn(ast::stmt_syntax *&self, ast::expr_syntax *exp)
{
    auto syntax = new ast::return_stmt_syntax;
    if(exp)
        syntax->exp = std::shared_ptr<ast::expr_syntax>(exp);
    self = static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalysePrimaryExpIntConst(ast::expr_syntax *&self, char *current_symbol)
{
    auto syntax = new ast::literal_syntax;
    syntax->intConst = std::stoi(current_symbol);
    self = static_cast<ast::expr_syntax*>(syntax);
}
//a-难度
void SynataxAnalyseStmtBlock(ast::stmt_syntax *&self, ast::block_syntax *block)
{
    self = static_cast<ast::stmt_syntax*>(block);
}

void SynataxAnalysePrimaryExpVar(ast::expr_syntax* &self, char* current_symbol)
{
    auto syntax = new ast::lval_syntax;
    syntax->name=current_symbol;
    syntax->restype=vartype::INT;
    self = static_cast<ast::expr_syntax*>(syntax);
}

void SynataxAnalyseVarDecl(ast::stmt_syntax *&self, ast::var_def_stmt_syntax *var_def, ast::var_decl_stmt_syntax *var_def_group)
{
     auto syntax = new ast::var_decl_stmt_syntax;
     if(var_def_group)
     {
        syntax->var_def_list=var_def_group->var_def_list;
     }
     syntax->var_def_list.insert(syntax->var_def_list.begin(),std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
     self=static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalyseVarDefGroup(ast::var_decl_stmt_syntax *&self, ast::var_def_stmt_syntax *var_def, ast::var_decl_stmt_syntax *var_def_group)
{
    auto syntax = new ast::var_decl_stmt_syntax;
     if(var_def_group)
     {
        syntax->var_def_list=var_def_group->var_def_list;
     }
     syntax->var_def_list.insert(syntax->var_def_list.begin(),std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
     self=syntax;
}

void SynataxAnalyseVarDef(ast::var_def_stmt_syntax *&self, char *ident, ast::expr_syntax *init)
{
     auto syntax = new ast::var_def_stmt_syntax;
     syntax->name=ident;
     syntax->initializer=std::shared_ptr<ast::expr_syntax>(init);
     self=syntax;
}

void SynataxAnalyseAddExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
     auto syntax = new ast::binop_expr_syntax;
     syntax->lhs=std::shared_ptr<ast::expr_syntax>(exp1);
     syntax->rhs=std::shared_ptr<ast::expr_syntax>(exp2);
     std::string op_str=op;
     syntax->op=op_str=="+"?binop::plus:binop::minus;
     syntax->restype=vartype::INT;
     self=static_cast<ast::expr_syntax*>(syntax);

}
//a难度
void SynataxAnalyseMulExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
     auto syntax = new ast::binop_expr_syntax;
     syntax->lhs=std::shared_ptr<ast::expr_syntax>(exp1);
     syntax->rhs=std::shared_ptr<ast::expr_syntax>(exp2);
     std::string op_str=op;
     syntax->op=op_str=="*"?binop::multiply :binop::divide;
     syntax->restype=vartype::INT;
     self=static_cast<ast::expr_syntax*>(syntax);
}

void SynataxAnalyseStmtAssign(ast::stmt_syntax *&self, ast::lval_syntax *target, ast::expr_syntax *value)
{
    auto syntax=new ast::assign_stmt_syntax;
    syntax->target=std::shared_ptr<ast::lval_syntax>(target);
    syntax->value=std::shared_ptr<ast::expr_syntax>(value);
    self=static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalyseLval(ast::lval_syntax *&self, char *ident)
{
    auto syntax=new ast::lval_syntax;
    syntax->name=ident;
    syntax->restype=vartype::INT;
    self=syntax;
}
//a+难度
void SynataxAnalyseStmtIf(ast::stmt_syntax *&self, ast::expr_syntax *cond, ast::stmt_syntax *then_body, ast::stmt_syntax *else_body)
{
    auto syntax=new ast::if_stmt_syntax;
    syntax->pred=std::shared_ptr<ast::expr_syntax>(cond);
    syntax->then_body=std::shared_ptr<ast::stmt_syntax>(then_body);
    syntax->else_body=std::shared_ptr<ast::stmt_syntax>(else_body);
    self=static_cast<ast::stmt_syntax*>(syntax);
}

void SynataxAnalyseLOrExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, ast::expr_syntax *cond2)
{
    auto syntax=new ast::logic_cond_syntax;
    syntax->op=relop::op_or;
    syntax->lhs=std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs=std::shared_ptr<ast::expr_syntax>(cond2);
    self=syntax;
}

void SynataxAnalyseLAndExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, ast::expr_syntax *cond2)
{
    auto syntax=new ast::logic_cond_syntax;
    syntax->op=relop::op_and;
    syntax->lhs=std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs=std::shared_ptr<ast::expr_syntax>(cond2);
    self=syntax;
}

void SynataxAnalyseEqExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, char *op, ast::expr_syntax *cond2)
{
    auto syntax=new ast::rel_cond_syntax;
    std::string op_str=op;
    syntax->op=op_str=="=="? relop::equal : relop::non_equal;
    syntax->lhs=std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs=std::shared_ptr<ast::expr_syntax>(cond2);
    self=syntax;
}


void SynataxAnalyseRelExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, char *op, ast::expr_syntax *exp)
{
    std::unordered_map<std::string,relop> mapping ={{"<",relop::less},{">",relop::greater},{"<=",relop::less_equal},{">=",relop::greater_equal}};
    auto syntax=new ast::rel_cond_syntax;
    std::string op_str=op;
    syntax->op=mapping.at(op_str);
    syntax->lhs=std::shared_ptr<ast::expr_syntax>(cond1);
    syntax->rhs=std::shared_ptr<ast::expr_syntax>(exp);
    self=syntax;

}

void SynataxAnalyseUnaryExp(ast::expr_syntax *&self, char *op, ast::expr_syntax *exp)
{
}