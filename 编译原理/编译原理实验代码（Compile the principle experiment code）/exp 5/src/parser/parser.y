%{
    #include "SyntaxTree.hpp"
    #include "SyntaxAnalyse.hpp"
    #include <iostream>

    int yylex();
    int yyparse();
    int yyrestart();
    
    extern FILE* yyin;
    extern char* yytext;
    extern int line_number;
    extern int column_end_number;
    extern int column_start_number;

    void yyerror(const char *s) {
        std::cerr << s << std::endl;
        std::cerr << "Error at line " << line_number << ": " << column_end_number << std::endl;
        std::cerr << "Error: " << yytext << std::endl;
        std::abort();
    }

    using namespace ast;
%}

%union {
    char* current_symbol; //we can't use string or any other object with construct function in union. 
    int symbol_size;
    struct ast::compunit_syntax *compunit ;
    struct ast::func_def_syntax *func_def;
    struct ast::expr_syntax *expr;
    struct ast::binop_expr_syntax *binop_expr;
    struct ast::unaryop_expr_syntax *unaryop_expr;
    struct ast::lval_syntax *lval;  
    struct ast::literal_syntax *literal;
    struct ast::stmt_syntax *stmt;
    struct ast::assign_stmt_syntax *assign;
    struct ast::block_syntax *block;
    struct ast::if_stmt_syntax *if_stmt;
    struct ast::return_stmt_syntax *return_stmt;
    struct ast::var_def_stmt_syntax *var_def_stmt;
    struct ast::var_decl_stmt_syntax *var_decl_stmt;
    enum vartype var_type;
}

%token <current_symbol> INT VOID IF ELSE RETURN Ident
%token <current_symbol> ADD SUB MUL DIV MOD
%token <current_symbol> LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%token <current_symbol> IntConst
%token <current_symbol> LESS GREATER EQUAL NOT
%token <current_symbol> LESS_EQUAL GREATER_EQUAL NOT_EQUAL AND OR
%token <current_symbol> ASSIGN COMMA SEMICOLON
%token <current_symbol> ERROR

%type <compunit> CompUnit
%type <func_def> FuncDef
%type <var_type> FuncType
%type <block> Block
%type <block> BlockItems
%type <stmt> Stmt
%type <expr> Exp
%type <stmt> Decl
%type <stmt> VarDecl 
%type <var_decl_stmt> VarDefGroup
%type <var_def_stmt> VarDef
%type <expr> InitVal
%type <expr> AddExp
%type <expr> PrimaryExp
%type <expr> MulExp
%type <expr> UnaryExp
%type <lval> Lval
%type <expr> Cond
%type <expr> LOrExp
%type <expr> LAndExp
%type <expr> EqExp
%type <expr> RelExp
%type <current_symbol> UnaryOp




%start CompUnit


%%

    CompUnit
    :CompUnit FuncDef { SyntaxAnalyseCompUnit($$,$1,$2);
    }
    |FuncDef { SyntaxAnalyseCompUnit($$,nullptr,$1); 
    }

    FuncDef
    :FuncType Ident LPAREN RPAREN Block { SyntaxAnalyseFuncDef($$,$1,$2,$5);}

    FuncType
    :VOID { SynataxAnalyseFuncType($$,$1);}
    |INT { SynataxAnalyseFuncType($$,$1);}

    Block
    : LBRACE BlockItems RBRACE { SynataxAnalyseBlock($$,$2);}

    BlockItems
    : BlockItems Stmt { SynataxAnalyseBlockItems($$,$1,$2);
    }
    | { SynataxAnalyseBlockItems($$,nullptr,nullptr);
    }
 /*a-难度---------------*/
    | BlockItems Decl{
        SynataxAnalyseBlockItems($$,$1,$2);
    }
 /*--------------------*/

    Stmt
    : RETURN Exp SEMICOLON { SynataxAnalyseStmtReturn($$,$2);}
 /*a-难度---------------*/
    | Block{
        SynataxAnalyseStmtBlock($$,$1);
    }
    |RETURN SEMICOLON{
        SynataxAnalyseStmtReturn($$,nullptr);
    }
 /*--------------------*/
 /*a难度---------------*/
    | Lval ASSIGN Exp SEMICOLON{
        SynataxAnalyseStmtAssign($$,$1,$3);
    }
 /*--------------------*/
 /*a+难度---------------*/
    | IF LPAREN Cond RPAREN Stmt {
        SynataxAnalyseStmtIf($$,$3,$5,nullptr);
    }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt{
        SynataxAnalyseStmtIf($$,$3,$5,$7);
    }
 /*--------------------*/

    PrimaryExp
    : IntConst { SynataxAnalysePrimaryExpIntConst($$,$1); }
 /*a-难度---------------*/
    | LPAREN Exp RPAREN{
        $$=$2;
    }
    | Ident{
        SynataxAnalysePrimaryExpVar($$,$1);
    }
 /*--------------------*/

 /*a-难度---------------*/
    Decl: VarDecl{
        $$=$1;
    }

    VarDecl: INT VarDef VarDefGroup SEMICOLON{
        SynataxAnalyseVarDecl($$,$2,$3);
    }

    VarDefGroup:  COMMA VarDef VarDefGroup{
        SynataxAnalyseVarDefGroup($$,$2,$3);
    }
    |{
        $$=nullptr;
    }

    VarDef: Ident {
         SynataxAnalyseVarDef($$,$1,nullptr);
    }
    | Ident ASSIGN InitVal{
        SynataxAnalyseVarDef($$,$1,$3);
    }
    InitVal: Exp{
        $$=$1;
    }

    AddExp: MulExp{
        $$=$1;
    }
    | AddExp ADD MulExp{
        SynataxAnalyseAddExp($$,$1,$2,$3);
    }
    | AddExp SUB MulExp{
        SynataxAnalyseAddExp($$,$1,$2,$3);
    }

    Exp: AddExp{
        $$=$1;
    }
 /*--------------------*/
 
 /*a难度---------------*/
    MulExp: UnaryExp{
        $$=$1;
    }
    | MulExp MUL UnaryExp {
         SynataxAnalyseMulExp($$,$1,$2,$3);
    }
    | MulExp DIV UnaryExp {
         SynataxAnalyseMulExp($$,$1,$2,$3);
    }

    Lval: Ident{
        SynataxAnalyseLval($$,$1);
    }
 /*--------------------*/
 
 /*a+难度---------------*/
   Cond: LOrExp{
    $$=$1;
   }

   LOrExp: LAndExp{
    $$=$1;
   }
   |LOrExp OR LAndExp{
    SynataxAnalyseLOrExp($$,$1,$3);
   }

    LAndExp: EqExp{
        $$=$1;
    }
    | LAndExp AND EqExp {
        SynataxAnalyseLAndExp($$,$1,$3);
    }

    EqExp: RelExp{
        $$=$1;
    }
    |EqExp EQUAL RelExp{
        SynataxAnalyseEqExp($$,$1,$2,$3);
    }
    |EqExp NOT_EQUAL RelExp{
        SynataxAnalyseEqExp($$,$1,$2,$3);
    }

    RelExp: AddExp{
        $$=$1;
    }
    | RelExp LESS AddExp {
        SynataxAnalyseRelExp($$,$1,$2,$3);
    }
    | RelExp GREATER AddExp {
        SynataxAnalyseRelExp($$,$1,$2,$3);
    }
    | RelExp LESS_EQUAL AddExp {
        SynataxAnalyseRelExp($$,$1,$2,$3);
    }
    | RelExp GREATER_EQUAL AddExp {
        SynataxAnalyseRelExp($$,$1,$2,$3);
    }

 /*--------------------*/

 /*a++难度---------------*/
    UnaryExp : PrimaryExp{
        $$=$1;
    }
    | UnaryOp UnaryExp{
        SynataxAnalyseUnaryExp($$,$1,$2);
    }

    UnaryOp:ADD{
        $$=$1;
    }
    | SUB{
        $$=$1;
    }
    | NOT{
        $$=$1;
    }
 /*--------------------*/

%%

