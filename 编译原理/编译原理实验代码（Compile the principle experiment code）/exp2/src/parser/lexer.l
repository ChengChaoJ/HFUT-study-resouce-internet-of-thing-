%option noyywrap
%{
    /*
        这部分会被原样拷贝到生成的cpp文件的开头
    */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include "SyntaxTree.hpp"
#include "parser.hpp"

int line_number;//行号
int column_start_number;//token开始的列号
int column_end_number;//token结束的列号

int current_token;

bool is_head_print;
void print_msg(std::ostream &out) {
    if(!is_head_print){
        out << std::setw(10) << "Token"
            << std::setw(10) << "Text"
            << std::setw(10) << "line"
            << std::setw(10) << "(s,e)"
            << std::endl;
            is_head_print = true;
    }
    
    out << std::setw(10) << current_token
        << std::setw(10) << yytext
        << std::setw(10) << line_number
        << std::setw(10) << "("  << column_start_number << "," << column_end_number << ")" 
        << std::endl;
    
}

int handle_token(int token) {
    current_token = token;    
    column_start_number = column_end_number;
    yylval.symbol_size = strlen(yytext);
    yylval.current_symbol = new char[yylval.symbol_size];
    strcpy(yylval.current_symbol,yytext);
    column_end_number += strlen(yytext);//yytext是正则表达式匹配的部分
    // print_msg(std::cout);
    return token;
}
%}

%%

\/\*([^\*]|\*[^\/])*\*+\/ {
    column_start_number = column_end_number;
    for (unsigned i = 0; i < strlen(yytext); i++) 
    {
        if (yytext[i] == '\n') 
        {
            line_number++;
            column_end_number = 0;
        } 
        else
            column_end_number++;
    }
}

\/\/.*  {
    column_start_number = column_end_number;
    column_end_number += strlen(yytext);
}

int { return handle_token( INT);}
void { return handle_token( VOID);}
if { return handle_token( IF);}
else { return handle_token( ELSE);}
return { return handle_token(RETURN);}
[a-zA-Z_][a-zA-Z_0-9]* { return handle_token( Ident);}
[0-9]+ { return handle_token( IntConst);}
"+" { return handle_token( ADD);}
"-" { return handle_token( SUB);}
"*" { return handle_token( MUL);}
"/" { return handle_token( DIV);}
"%" { return handle_token( MOD);}
"(" { return handle_token( LPAREN);}
")" { return handle_token( RPAREN);}
"[" { return handle_token( LBRACKET);}
"]" { return handle_token( RBRACKET);}
"{" { return handle_token( LBRACE);}
"}" { return handle_token( RBRACE);}
"<" { return handle_token( LESS);}
">" { return handle_token( GREATER);}
"==" { return handle_token( EQUAL);}
"!" { return handle_token( NOT);}
"<=" {return handle_token( LESS_EQUAL);}
">=" {return handle_token( GREATER_EQUAL);}
"!=" {return handle_token( NOT_EQUAL);}
"&&" {return handle_token( AND);}
"||" {return handle_token( OR);}
"=" {return handle_token( ASSIGN);}
"," {return handle_token( COMMA);}
";" { return handle_token( SEMICOLON);}

\n {
    line_number++;
    column_start_number = column_end_number;
    column_end_number = 0;
}


" "|\r {
    column_start_number = column_end_number;
    column_end_number += strlen(yytext);
}

\t {
    column_start_number = column_end_number;
    column_end_number += 4;
}

. { return handle_token(ERROR); }

%%