#include "parser/SyntaxTree.hpp"
#include "ir/irbuilder.hpp"
#include "ir/ir_printer.hpp"
#include <fstream>
#include <stdlib.h>
ast::SyntaxTree syntax_tree;
int main(){
    ast::parse_file(std::cin);
    std::shared_ptr<ir::IrBuilder> irbuilder = std::make_shared<ir::IrBuilder>();
    syntax_tree.accept(*irbuilder);
    std::shared_ptr<ir::IrPrinter> irprinter = std::make_shared<ir::IrPrinter>();
    irbuilder->compunit->accept(*irprinter);
}
