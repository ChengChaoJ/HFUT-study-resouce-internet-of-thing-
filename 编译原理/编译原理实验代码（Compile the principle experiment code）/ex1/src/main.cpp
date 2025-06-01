#include "parser/SyntaxTree.hpp"
#include <fstream>
#include <stdlib.h>
ast::SyntaxTree syntax_tree;
int main(){
    ast::parse_file(std::cin);
}