#include "SyntaxTree.hpp"
#include <unordered_map>
#include <utility>
//a--难度
void SyntaxAnalyseCompUnit(ast::compunit_syntax* &self, ast::compunit_syntax* compunit, ast::func_def_syntax* func_def);
void SyntaxAnalyseFuncDef(ast::func_def_syntax* &self, vartype var_type, char* Ident,ast::block_syntax * block);
void SynataxAnalyseFuncType(vartype &self, char* type);
void SynataxAnalyseBlock(ast::block_syntax* &self, ast::block_syntax* block_items);
void SynataxAnalyseBlockItems(ast::block_syntax* &self,ast::block_syntax* block_items, ast::stmt_syntax* stmt);
void SynataxAnalyseStmtReturn(ast::stmt_syntax* &self, ast::expr_syntax* exp);
void SynataxAnalysePrimaryExpIntConst(ast::expr_syntax* &self, char* current_symbol);
//a-难度
void SynataxAnalyseStmtBlock(ast::stmt_syntax* &self, ast::block_syntax *block);
void SynataxAnalysePrimaryExpVar(ast::expr_syntax* &self, char* current_symbol);
void SynataxAnalyseVarDecl(ast::stmt_syntax* &self, ast::var_def_stmt_syntax *var_def,ast::var_decl_stmt_syntax *var_def_group);
void SynataxAnalyseVarDefGroup(ast::var_decl_stmt_syntax * &self, ast::var_def_stmt_syntax *var_def,ast::var_decl_stmt_syntax *var_def_group);
void SynataxAnalyseVarDef(ast::var_def_stmt_syntax *&self,char* ident,ast::expr_syntax* init);
void SynataxAnalyseAddExp(ast::expr_syntax* &self,ast::expr_syntax* exp1,char * op,ast::expr_syntax* exp2);
//a难度
void SynataxAnalyseMulExp(ast::expr_syntax* &self,ast::expr_syntax* exp1,char * op,ast::expr_syntax* exp2);
void SynataxAnalyseStmtAssign(ast::stmt_syntax *&self,ast::lval_syntax* target,ast::expr_syntax* value);
void SynataxAnalyseLval(ast::lval_syntax *&self,char *ident);
//a+难度
void SynataxAnalyseStmtIf(ast::stmt_syntax *&self,ast::expr_syntax *cond,ast::stmt_syntax *then_body,ast::stmt_syntax *else_body);
void SynataxAnalyseLOrExp(ast::expr_syntax* &self,ast::expr_syntax* cond1,ast::expr_syntax* cond2);
void SynataxAnalyseLAndExp(ast::expr_syntax* &self,ast::expr_syntax* cond1,ast::expr_syntax* cond2);
void SynataxAnalyseEqExp(ast::expr_syntax* &self,ast::expr_syntax* cond1,char * op,ast::expr_syntax* cond2);
void SynataxAnalyseRelExp(ast::expr_syntax* &self,ast::expr_syntax *cond1,char * op,ast::expr_syntax *exp);
//a++难度
void SynataxAnalyseUnaryExp(ast::expr_syntax* &self,char * op,ast::expr_syntax* exp);