#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
 
using std::cout;
using std::string;
using std::vector;

template <typename T>
using ptr = std::shared_ptr<T>; //智能指针

template <typename T>
using ptr_list = std::vector<ptr<T>>;   //智能指针数组

  //一些枚举类
enum class relop    //关系操作符
{
    equal = 0,
    non_equal,
    less,
    less_equal,
    greater,
    greater_equal,
    op_and,
    op_or
};
enum class binop    //二元操作符
{
    plus = 0,
    minus,
    multiply,
    divide,
    modulo
};

enum class unaryop  //一元操作符
{
    plus = 0,
    minus,
    op_not
};
enum class vartype//类型
{
  INT=0,
  VOID,
  FLOAT,
  FLOATADDR,
  INTADDR,
  BOOL
};

//ast结点
namespace ast{

struct syntax_tree_node;
struct compunit_syntax;
struct func_def_syntax;
struct expr_syntax;
struct expr_syntax;
struct binop_expr_syntax;
struct unaryop_expr_syntax;
struct lval_syntax;  
struct literal_syntax;
struct stmt_syntax;
struct assign_stmt_syntax;
struct block_syntax;
struct if_stmt_syntax;
struct return_stmt_syntax;
struct var_def_stmt_syntax;
struct var_decl_stmt_syntax;
//访问者模板
struct syntax_tree_visitor;//访问者模板
//语法树本树


//ast结点
struct syntax_tree_node {
  public:
    int line;
    //用于访问者模式
    virtual void accept(syntax_tree_visitor &visitor) = 0;
    //打印
    virtual void print() = 0;
};

class SyntaxTreePrinter {
    public:
    int cur_level = 0;
    void LevelPrint(std::ostream &out,std::string type_name,bool is_terminal){
        for(int i = 0 ; i < cur_level; ++i) out << "|  ";
        out << ">--" << (is_terminal ? "*" : "+") << type_name;
        out << std::endl;
    }
};

class SyntaxTree {
  public:
    syntax_tree_node *root;
    void print() { //
        this->root->print();
    }
    void accept(syntax_tree_visitor &visitor){
        this->root->accept(visitor);
    };
};

//编译单位，就是一个文件
struct compunit_syntax : syntax_tree_node
{
    ptr_list<func_def_syntax> global_defs;//指针数组，存多个 函数定义
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

// 函数定义
struct func_def_syntax : syntax_tree_node
{
    vartype rettype;    //返回类型
    std::string name;   //名字
    ptr<block_syntax> body; //函数体，为指向block_syntax的指针
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//表达式
struct expr_syntax : virtual syntax_tree_node
{
    virtual void accept(syntax_tree_visitor &visitor) = 0;
    virtual void print() = 0;
};

//条件表达式
struct logic_cond_syntax: expr_syntax   //逻辑(与或)判断语句
{
    relop op;
    ptr<expr_syntax> lhs;
    ptr<expr_syntax> rhs;
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};
struct rel_cond_syntax: expr_syntax //关系(大于小于)判断语句
{
    relop op;
    ptr<expr_syntax> lhs;
    ptr<expr_syntax> rhs;
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};



//二元算术表达式
struct binop_expr_syntax : expr_syntax
{
    binop op;   //二元运算符
    ptr<expr_syntax> lhs, rhs;  //左操作数，右操作数
    vartype restype;            //结果类型
    int intConst;   //值
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//一元算术表达式
struct unaryop_expr_syntax : expr_syntax
{
    unaryop op; //一元操作符
    ptr<expr_syntax> rhs;   //右操作数
    vartype restype;    //结果类型
    int intConst;   //值
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//求值表达式，比如算术表达式中的一个变量a
struct lval_syntax : expr_syntax
{
    std::string name;   //名字
    vartype restype;    //类型
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//常数
struct literal_syntax : expr_syntax
{
    vartype restype;//类型
    int intConst;//数值
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//比如if,定义，赋值（语句？
struct stmt_syntax : virtual syntax_tree_node
{
    virtual void accept(syntax_tree_visitor &visitor) = 0;
    virtual void print() = 0;
};
//变量定义，可以有初始值也可以没有
struct var_def_stmt_syntax : stmt_syntax
{
    vartype restype;    //类型
    std::string name;   //名字
    ptr<expr_syntax> initializer;   //初始值
    //为了处理值改变的问题，自己改了
    bool haveInit = false;
    int value;
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

struct var_decl_stmt_syntax : stmt_syntax
{
    ptr_list<var_def_stmt_syntax> var_def_list;//变量定义数组
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//赋值语句
struct assign_stmt_syntax : stmt_syntax
{
    ptr<lval_syntax> target;
    ptr<expr_syntax> value;
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//大括号里面的东西
struct block_syntax : stmt_syntax
{
    ptr_list<stmt_syntax> body; //多条stmt_syntax组成的的数组
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

// If statement.
struct if_stmt_syntax : stmt_syntax
{
    ptr<expr_syntax> pred;  //判断语句
    ptr<stmt_syntax> then_body; //if判断成功执行的语句
    ptr<stmt_syntax> else_body; //else语句
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//return语句
struct return_stmt_syntax: stmt_syntax
{
    ptr<expr_syntax> exp;//表达式
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};

//空语句
struct empty_stmt_syntax : stmt_syntax
{
    virtual void accept(syntax_tree_visitor &visitor) override final;
    virtual void print() override final;
};


//访问者模板
class syntax_tree_visitor//暂时好像没用？
{
  public:
    virtual void visit(compunit_syntax &node) = 0;
    virtual void visit(func_def_syntax &node) = 0;
    virtual void visit(rel_cond_syntax &node) = 0;
    virtual void visit(logic_cond_syntax &node) = 0;
    virtual void visit(binop_expr_syntax &node) = 0;
    virtual void visit(unaryop_expr_syntax &node) = 0;
    virtual void visit(lval_syntax &node) = 0;
    virtual void visit(literal_syntax &node) = 0;
    virtual void visit(var_def_stmt_syntax &node) = 0;
    virtual void visit(assign_stmt_syntax &node) = 0;
    virtual void visit(block_syntax &node) = 0;
    virtual void visit(if_stmt_syntax &node) = 0;
    virtual void visit(return_stmt_syntax &node) = 0;
    virtual void visit(var_decl_stmt_syntax &node) = 0;
};

void parse_file(string input_file_path);
void parse_file(std::istream& in);


}//end namespace ast

#endif