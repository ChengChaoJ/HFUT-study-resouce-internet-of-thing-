#ifndef IR_PRINTER_HPP
#define IR_PRINTER_HPP

#include "ir.hpp"
#include <variant>

namespace ir {

class IrPrinter : public ir_visitor {
public:
   std::unordered_map<std::variant<vartype,unaryop,binop,relop>,std::string> mapping = { //哈希表，记录 变量类型、运算符对应的字符串表示
    //std::variant<vartype,binop,relop> 用于存储多种可能的类型中的一个   
        {vartype::FLOAT, "float"},
        {vartype::INT,  "i32"},
        {vartype::VOID , "void"},
        {vartype::FLOATADDR, "float*"},
        {vartype::INTADDR, "i32*"},
        {unaryop::plus, "add"},
        {unaryop::minus, "sub"},
        {unaryop::op_not, "not"},
        {binop::plus, "add"},
        {binop::minus,  "sub"},
        {binop::multiply , "mul"},
        {binop::divide, "sdiv"},
        {binop::modulo, "mod"},
        {relop::greater, "sgt"},
        {relop::less,  "slt"},
        {relop::greater_equal , "sge"},
        {relop::less_equal, "sle"},
        {relop::equal, "eq"},
        {relop::non_equal, "ne"},
        {relop::op_and, "and"},
        {relop::op_or, "or"}
    };
   std::unordered_map<vartype, std::string> base_type = {//用于存储地址类型的基本类型名称
       {vartype::FLOATADDR, "float"},
       {vartype::INTADDR, "i32"}
   };
   std::ostream &out;

   IrPrinter(std::ostream &out = std::cout) : out(out){};//使用参数out来初始化成员变量out
   virtual void visit(ir_reg &node) override final;
   virtual void visit(ir_constant &node) override final;
   virtual void visit(ir_basicblock &node) override final;
   virtual void visit(ir_module &node) override;
   virtual void visit(ir_userfunc &node) override final;
   virtual void visit(store &node) override final;
   virtual void visit(jump &node) override final;
   virtual void visit(br &node) override final;
   virtual void visit(ret &node) override final;
   virtual void visit(load &node) override final;
   virtual void visit(alloc &node) override final;
   virtual void visit(phi &node) override final;
   virtual void visit(unary_op_ins &node) override final;
   virtual void visit(binary_op_ins &node) override final;
   virtual void visit(cmp_ins &node) override final;
   virtual void visit(logic_ins &node) override final;
   std::string get_value(const ptr<ir::ir_value> &val);
};

} // namespace ir



#endif