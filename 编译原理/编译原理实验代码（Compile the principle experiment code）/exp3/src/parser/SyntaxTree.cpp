#include "SyntaxTree.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <cassert>

using namespace ast;

SyntaxTreePrinter ast_printer;

void ast::parse_file(string input_file_path) {
    const char *input_file_path_cstr = input_file_path.c_str();
    if (input_file_path != "") {
        auto input_file = fopen(input_file_path_cstr, "r");
        if (!input_file) {
            std::cout << "Error: Cannot open file " << input_file_path_cstr << "\n";
            perror("fault");
            exit(110);
        }
        std::ifstream t(input_file_path);
        std::string buffer;
        //预设缓冲区大小
        t.seekg(0, std::ios::end);
        buffer.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        buffer.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        //自带的函数
        yy_scan_string(buffer.c_str());
        // yyrestart(buffer_file);
        fclose(input_file);
    }
    //开始语法分析，用的是自带的函数
    yyparse();
}
void ast::parse_file(std::istream &in)
{
    std::string buffer;
    std::string str;
    while(in.peek() != EOF){
        std::getline(in,str);
        buffer += str;
        buffer += '\n';
    } 
    yy_scan_string(buffer.c_str());
    yyparse();
}


void compunit_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void compunit_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"CompUnit",false);
    ast_printer.cur_level++;
    for(auto child : this->global_defs){
        child.get()->print();
    }
    ast_printer.cur_level--;
}

void func_def_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void func_def_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"FuncDef",false);

    ast_printer.cur_level++;
    std::string type = (this->rettype == vartype::VOID ? "void" : "int");
    ast_printer.LevelPrint(std::cout,type,true);
    ast_printer.LevelPrint(std::cout,this->name,true);
    ast_printer.LevelPrint(std::cout,"(",true);
    ast_printer.LevelPrint(std::cout,")",true);
    ast_printer.LevelPrint(std::cout,"{",true);
    
    ast_printer.cur_level++;
    this->body.get()->print();
    ast_printer.cur_level--;

    ast_printer.LevelPrint(std::cout,"}",true);
    ast_printer.cur_level--;
}

void binop_expr_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void binop_expr_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"binop",false);
    ast_printer.cur_level++;
    this->lhs->print();
    std::vector<std::string> op={"+","-","*","/","%"};
    ast_printer.LevelPrint(std::cout,op[int(this->op)],true);
    this->rhs->print();
    ast_printer.cur_level--;
}

void unaryop_expr_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void unaryop_expr_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"unary",false);
    ast_printer.cur_level++;
    std::vector<std::string> op={"+","-","!"};
    ast_printer.LevelPrint(std::cout,op[int(this->op)],true);
    this->rhs->print();
    ast_printer.cur_level--;
}

void lval_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void lval_syntax::print()
{
    ast_printer.LevelPrint(std::cout,this->name,true);

}

void literal_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void literal_syntax::print()
{
    // ast_printer.LevelPrint(std::cout,"IntConst",false);
    // ast_printer.cur_level++;
    ast_printer.LevelPrint(std::cout,std::to_string(this->intConst),true);
    // ast_printer.cur_level--;
}

void var_def_stmt_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void var_def_stmt_syntax::print()
{

    ast_printer.LevelPrint(std::cout,this->name,true);
    
    if(this->initializer)
    {
        ast_printer.LevelPrint(std::cout,"=",true);
        this->initializer->print();
    }
        
}

void assign_stmt_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void assign_stmt_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"assign_stmt",false);
    ast_printer.cur_level++; 
    this->target->print();
    ast_printer.LevelPrint(std::cout,"=",true);
    this->value->print();
    ast_printer.LevelPrint(std::cout,";",true);
    ast_printer.cur_level--; 
}

void block_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void block_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"Block",false);
    ast_printer.cur_level++;
    for(auto & content: this->body){
        content.get()->print();
    }
    ast_printer.cur_level--;
}

void if_stmt_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void if_stmt_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"if_stmt",false);
    ast_printer.cur_level++;
    ast_printer.LevelPrint(std::cout,"(",true);
    this->pred->print();
    ast_printer.LevelPrint(std::cout,")",true);
    ast_printer.LevelPrint(std::cout,"then_block",false);
    ast_printer.cur_level++;
    if(this->then_body)
        this->then_body->print();
    ast_printer.cur_level--;
    if(this->else_body)
    {
         ast_printer.LevelPrint(std::cout,"else_body",false);
         ast_printer.cur_level++;
         this->else_body->print();
         ast_printer.cur_level--;
    }
    ast_printer.cur_level--;
}

void return_stmt_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void return_stmt_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"stmt",false);

    ast_printer.cur_level++;
    ast_printer.LevelPrint(std::cout,"return",true);
    this->exp.get()->print();
    ast_printer.LevelPrint(std::cout,";",true);
    ast_printer.cur_level--;
}

void empty_stmt_syntax::accept(syntax_tree_visitor &visitor)
{
}

void empty_stmt_syntax::print()
{
}

void ast::var_decl_stmt_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void ast::var_decl_stmt_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"decl",false);
    ast_printer.cur_level++;
    std::vector<std::string> typeMapping = {"int","void","float"};

    auto type = this->var_def_list.back()->restype;

    ast_printer.LevelPrint(std::cout,typeMapping[int(type)],true);
    for(int i = 0; i < this->var_def_list.size(); ++i){
        
        var_def_list[i]->print();

        if(i != this->var_def_list.size() - 1){
            ast_printer.LevelPrint(std::cout,",",true);
        }else{
            ast_printer.LevelPrint(std::cout,";",true);
        }
    }
    ast_printer.cur_level--;
}

void ast::logic_cond_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void ast::logic_cond_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"logic_cond",false);
    ast_printer.cur_level++;
    this->lhs->print();
    std::vector<std::string> op={"==","!=","<","<=",">",">=","&&","||"};
    ast_printer.LevelPrint(std::cout,op[int(this->op)],true);
    this->rhs->print();
    ast_printer.cur_level--;
}

void ast::rel_cond_syntax::accept(syntax_tree_visitor &visitor)
{
    visitor.visit(*this);
}

void ast::rel_cond_syntax::print()
{
    ast_printer.LevelPrint(std::cout,"rel_cond",false);
    ast_printer.cur_level++;
    this->lhs->print();
    std::vector<std::string> op={"==","!=","<","<=",">",">=","&&","||"};
    ast_printer.LevelPrint(std::cout,op[int(this->op)],true);
    this->rhs->print();
    ast_printer.cur_level--;
}
