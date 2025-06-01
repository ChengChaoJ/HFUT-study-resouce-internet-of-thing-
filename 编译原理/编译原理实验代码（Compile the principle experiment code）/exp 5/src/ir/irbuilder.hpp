#ifndef IR_BUILDER_HPP
#define IR_BUILDER_HPP

#include <vector>
#include <utility>
#include <variant>

#include "parser/SyntaxTree.hpp"
#include "ir/symbol_table.hpp"
#include "ir/ir.hpp"


template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T>
using ptr_list = std::vector<ptr<T>>;
#define Case(T, a, b) if (auto a = std::dynamic_pointer_cast<T>(b))

namespace ir {

enum class Mode {
    conditon,
    normal
};

class IrBuilder : public ast::syntax_tree_visitor {

    std::string cur_func_name;//当前函数名
    ptr<ir::ir_userfunc> cur_func;//当前函数的IR对象
    ptr<ir::ir_basicblock> cur_block, return_bb;//当前BB，当前函数的return BB
    ptr<ir::ir_value> pass_value;//用于
    std::vector<std::pair<ptr<ir::ir_value>, ptr<ir::ir_basicblock >>> return_value;//可能的返回值和对应的BB
    ptr<ir::ir_memobj> ir_value;
    bool found_main;//是否找到了主函数
    Mode mode;
//访问者模式
 public:
    
    ptr<ir::ir_module> compunit;
    symbol_table::FunctionTable functions;//函数表
    symbol_table::Scope scope;//变量表
    virtual void visit(ast::compunit_syntax &node) override final;
    virtual void visit(ast::func_def_syntax &node) override final;
    virtual void visit(ast::rel_cond_syntax &node) override final;
    virtual void visit(ast::logic_cond_syntax &node) override final;
    virtual void visit(ast::binop_expr_syntax &node) override final;
    virtual void visit(ast::unaryop_expr_syntax &node) override final;
    virtual void visit(ast::lval_syntax &node) override final;
    virtual void visit(ast::literal_syntax &node) override final;
    virtual void visit(ast::var_def_stmt_syntax &node) override final;
    virtual void visit(ast::assign_stmt_syntax &node) override final;
    virtual void visit(ast::block_syntax &node) override final;
    virtual void visit(ast::if_stmt_syntax &node) override final;
    virtual void visit(ast::return_stmt_syntax &node) override final;
    virtual void visit(ast::var_decl_stmt_syntax &node) override final;
    ptr<ir::ir_reg> get_value(ptr<ir::ir_reg> addr);
};

}
#endif