#ifndef IR_HPP
#define IR_HPP

#include "../parser/SyntaxTree.hpp"
#include <variant>
#include <list>
#include <optional>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <functional>

const int i32_size = 4;
namespace LoongArch{
    class ProgramBuilder;
    class IrMapping;
    class ColoringAllocator;
}
namespace Pass {
struct LiveInterval {
    int l, r;
};
} // namespace Pass

namespace ir {

class IrBuilder;
class ir_visitor;//用于实现访问者模式
class IrPrinter;
class ir_func;//函数
class ir_userfunc;//用户定义的函数
class ir_libfunc;//库函数

class ir_value;//值 包括ir_reg  ir_constant jumpList
class ir_reg;//虚拟寄存器
class ir_constant;//常量值

class ir_basicblock;//基本块
class ir_memobj;//内存对象，用于表示程序中的数据结构或变量在内存中的布局

class alloc;//内存分配指令
class binary_op_ins;//二元操作指令
class br;//分支指令，用于实现条件或无条件跳转
class store;//存储指令
class phi;//PHI 节点 用于处理多路分支时的变量合并
class jump;//无条件跳转指令
class load;//加载指令，用于从内存中读取数据
class ret;//返回指令
class cmp_ins;//比较指令
class control_ins;//控制指令的基类或接口

class printable {
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
};

class ir_value {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
public:
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
};

class ir_reg : public ir_value {
    friend IrBuilder;
    friend IrPrinter;
    friend LoongArch::IrMapping;
    friend LoongArch::ProgramBuilder;
private:
    int id;     //virtual register id
    vartype type;  //int or float (extension)
    int size;   //some byte
public:
    ir_reg(int id,vartype type,int size) : id(id) , type(type), size(size) {}
    bool operator==(ir_reg& rhs) {return id == rhs.id;}
    bool operator<(ir_reg& rhs) {return id < rhs.id;}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class ir_constant : public ir_value {
    friend IrBuilder;
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
private:
    vartype type;
    std::optional<std::variant<int,float>> init_val;
    //std::optional 表示这个值可以存在或不存在
    //std::variant 表示这个值可以是整数或浮点数
public:
    ir_constant(std::optional<std::variant<int,float>> init_val) : init_val(init_val) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class jumpList : public ir_value { //表示与跳转相关的信息
    friend IrBuilder;
    std::vector<ptr<ir_basicblock>*> trueList;//当条件为真时可能跳转的目标基本块列表
    
    std::vector<ptr<ir_basicblock>*> falseList;//当条件为假时可能跳转的目标基本块列表
   
public:
    virtual void accept(ir_visitor& visitor);
    virtual void print(std::ostream & out = std::cout);
};

class ir_memobj : public printable {
    friend IrBuilder;
    friend IrPrinter;
protected:
    std::string name;//内存对象的名称
    std::shared_ptr<ir_reg> addr;//内存对象的地址
    int size;//内存对象的大小
public:
    //获取内存对象的地址
    ptr<ir_reg> get_addr() { return this->addr;};

    //获取内存对象的大小
    int get_size() {return this->size;}

    ir_memobj(std::string name , ptr<ir_reg> addr, int size) : name(name) , addr(addr) , size(size) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class ir_func_arg : public ir_memobj {  //用于表示函数的参数
    int id;
    //由于accept 和 print 函数都被声明为final，将使用其基类的实现
};

class ir_scope : public printable { //表示程序中的一个作用域
    friend LoongArch::ProgramBuilder;
    friend ir_userfunc;
    std::list<std::shared_ptr<ir_memobj>> ir_objs;//存储作用域内的所有内存对象，例如局部变量或函数参数
public:
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class ir_instr : public printable { //表示程序中的一个指令
public:
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;

    //返回一个 ir::ir_value 智能指针的向量,表示当前指令所使用的寄存器或值
    virtual std::vector<ptr<ir::ir_value>> use_reg() = 0;
    //返回一个 ir::ir_value 智能指针的向量,表示当前指令所定义的寄存器或值 
    virtual std::vector<ptr<ir::ir_value>> def_reg() = 0; 
};

class ir_basicblock : public printable {
    friend IrPrinter;   
    friend LoongArch::ProgramBuilder; 
    std::string name;
    int id;
    std::list<std::shared_ptr<ir_instr>> instructions;//指令列表
public:
    ir_basicblock(int id) : id(id) { name = "bb"+std::to_string(id); };

    //插入新的ir_instr
    void push_back(ptr<ir_instr> inst);

    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    bool operator<(ir::ir_basicblock & rhs){return this->id < rhs.id;}

    //返回基本块中最后一个指令的智能指针
    std::shared_ptr<ir_instr> getLastInst();
    //返回基本块中第一个指令的智能指针
    std::shared_ptr<ir_instr> getFirstInst();
    std::string getName();
    void for_each(std::function<void(std::shared_ptr<ir::ir_instr> inst)> f,bool isReverse);
    //使用std::function对象为参数，可以接收任何可调用对象
    //包括函数指针、lambda 表达式、函数对象以及绑定表达式
    //std::function<void(std::shared_ptr<ir::ir_instr> inst)> f
    //f为function对象，function中void表示函数返回类型是void，inst是参数
    //f是可以存储任何接受一个std::shared_ptr<ir::ir_instr>类型参数并返回void的可调用对象的函数对象
};

class ir_func : public printable {
protected:
    std::string name;
    vartype rettype;//返回类型
    std::unordered_map<int,ir_func_arg> args;//参数哈希表
public:
    ir_func(std::string name) : name(name) {}
    bool set_retype(vartype rettype);//设置返回类型
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
};

class ir_module : public printable {    //模块
    friend IrPrinter;
protected:
    std::unique_ptr<ir_scope> scope;//用于存储模块的作用域信息
    //unique_ptr拥有对象的独占所有权

public:    
    std::list<std::pair<std::string,std::shared_ptr<ir_userfunc>>> usrfuncs;
    //pair对组列表 <函数名,智能指针指向函数对象>
    std::list<std::pair<std::string,std::shared_ptr<ir_libfunc>>> libfuncs;
    ir_module(){
        this->scope = std::make_unique<ir_scope>();
    }
    ptr<ir_userfunc> new_func(std::string name);//创建一个新的用户定义函数
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual void reg_allocate();//用于寄存器分配的逻辑
};

//below is func

class ir_libfunc : public ir_func {

public:
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
};

class ir_userfunc : public ir_func , public std::enable_shared_from_this<ir_userfunc> {
//提供了std::enable_shared_from_this的支持,
//这意味着 ir_userfunc 对象可以被 std::shared_ptr 管理
public:
    friend IrPrinter;
    friend IrBuilder;
    friend LoongArch::ProgramBuilder;
private:
    std::unique_ptr<ir_scope> scope; //存储函数的作用域信息
    std::list<std::shared_ptr<ir_basicblock>> bbs;//基本块指针列表
    int max_reg, max_bb;    //跟踪寄存器和基本块的最大数量
    ptr<ir_basicblock> entry;   //入口基本块

    //存储指令的行号信息
    std::unordered_map<int,std::shared_ptr<ir::ir_instr>> revInstLineNumber;
    //用于快速查找指令的行号
    std::unordered_map<std::shared_ptr<ir::ir_instr>,int> instLineNumber;
    //用于存储寄存器分配的结果
    std::unordered_map<std::shared_ptr<ir::ir_reg>,int> regAllocateOut;
public:
    ir_userfunc(std::string name); 
    //用于创建新的内存对象
    ptr<ir_memobj> new_obj(std::string name);
    //自动创建序号递增的寄存器
    ptr<ir_reg> new_reg(vartype type);
    //创建BB
    ptr<ir_basicblock> new_block();

    virtual void accept(ir_visitor& visitor);
    virtual void print(std::ostream & out = std::cout) override;
    
    //获取寄存器或值的生命周期间隔信息
    std::unordered_map<ptr<ir::ir_value>,Pass::LiveInterval> GetLiveInterval();
    //获取基本块的线性序列
    std::vector<std::shared_ptr<ir_basicblock>> GetLinerSequence();
    //用于寄存器分配的逻辑
    virtual void reg_allocate(); 
};

//below is instruction
class reg_write_ins : public ir_instr { //寄存器写入指令

    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override;
};

class control_ins : public ir_instr {   //控制流指令，如跳转、条件分支
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override;
};

class store : public ir_instr { //存储指令
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_value> addr;//存储操作中的地址
    ptr<ir_value> value;//存储操作中的值
public:
    store(ptr<ir_value> addr,ptr<ir_value> value):addr(addr),value(value){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class jump : public control_ins {   //无条件跳转指令
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_basicblock> target;  //表示跳转基本块目标
public:
    jump(ptr<ir_basicblock> target):target(target){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;

    //获得目标基本块目标
    std::shared_ptr<ir::ir_basicblock> getTarget();
};

class br : public control_ins { //条件分支（branch）指令
    friend IrBuilder;
    friend IrPrinter;
    ptr<ir_value> cond; //条件分支的条件
    ptr<ir_basicblock>target_true, target_false;//条件为真和假时跳转的目标基本块
public:
    //返回条件分支为真时的目标基本块
    ptr<ir_basicblock> get_target_true() {return this->target_true;};
    //返回条件分支为假时的目标基本块
    ptr<ir_basicblock> get_target_false() { return this->target_false;};
    //设置条件分支为真时的目标基本块
    void set_target_true(ptr<ir_basicblock> bb){this->target_true=bb;}
    //设置条件分支为假时的目标基本块
    void set_target_false(ptr<ir_basicblock> bb){this->target_false=bb;}

    //构造函数
    br(ptr<ir_value> cond,ptr<ir_basicblock> target_true,ptr<ir_basicblock> target_false):cond(cond), target_true(target_true),target_false(target_false){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class ret : public control_ins {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_value> value;//返回值
    bool has_return_value;//是否有返回值
public:
    ret(ptr<ir_value> value, bool has_return_value):value(value),has_return_value(has_return_value){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class load : public reg_write_ins {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_reg> dst;//加载操作的目标寄存器
    ptr<ir_value>  addr;//加载操作的内存地址
public:
    load(ptr<ir_reg> dst,ptr<ir_value> addr): dst(dst),addr(addr){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class alloc : public reg_write_ins {//内存分配（allocation）指令
    friend IrPrinter;
    ptr<ir_memobj> var;//分配的内存对象
public:
    alloc(ptr<ir_memobj>  var):var(var){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class phi : public reg_write_ins {//Phi（φ）函数 用于在多个控制流路径汇聚到一个点时，合并不同路径上的变量值的一种机制
public:
    friend IrPrinter;    
    friend class IrBuilder;
    std::vector<std::pair<ptr<ir_value>,ptr<ir_basicblock>>> uses;//存储Phi节点的输入值和对应的控制流路径（基本块）向量
    ptr<ir_reg> dst;   //目标寄存器
public:
    phi(ptr<ir_reg> dst): dst(dst) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class unary_op_ins : public reg_write_ins {
    friend IrPrinter;    
    std::shared_ptr<ir_value> dst, src;//目标（结果存储位置）和源（操作数）
    unaryop op;//操作符
public:
    unary_op_ins(ptr<ir_value> dst,ptr<ir_value> src,unaryop op) : dst(dst), src(src), op(op) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class binary_op_ins : public reg_write_ins {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    std::shared_ptr<ir_value> dst, src1,src2;
    binop op;
public:
    binary_op_ins(ptr<ir_value> dst,ptr<ir_value> src1,ptr<ir_value> src2,binop op) : dst(dst), src1(src1), src2(src2), op(op) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class cmp_ins : public reg_write_ins {
    friend IrPrinter;
    std::shared_ptr<ir_value> src1,src2;//比较操作的两个操作数
    std::shared_ptr<ir_reg> dst;//比较结果存储的目标寄存器
    relop op;
public:
    cmp_ins(ptr<ir_reg> dst,ptr<ir_value> src1,ptr<ir_value> src2,relop op) : dst(dst), src1(src1), src2(src2), op(op) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class logic_ins : public reg_write_ins {
    friend IrPrinter;
    std::shared_ptr<ir_value> dst, src1,src2;
    relop op;
public:
    logic_ins(ptr<ir_value> dst,ptr<ir_value> src1,ptr<ir_value> src2,relop op) : dst(dst), src1(src1), src2(src2), op(op) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class nop : public ir_instr { // 无操作指令
public:
    nop() {}
    virtual void accept(ir_visitor& visitor) override final {

    }
    virtual void print(std::ostream & out = std::cout) override final {

    }
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final {
        return {};
    }
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final {
        return {};
    }
};


class ir_visitor {
public:
    virtual void visit(ir_reg &node) = 0;
    virtual void visit(ir_constant &node) = 0;
    virtual void visit(ir_basicblock &node) = 0;
    virtual void visit(ir_module &node) = 0;
    virtual void visit(ir_userfunc &node) = 0;
    virtual void visit(store &node) = 0;
    virtual void visit(jump &node) = 0;
    virtual void visit(br &node) = 0;
    virtual void visit(ret &node) = 0;
    virtual void visit(load &node) = 0;
    virtual void visit(alloc &node) = 0;
    virtual void visit(phi &node) = 0;
    virtual void visit(unary_op_ins &node) = 0;
    virtual void visit(binary_op_ins &node) = 0;
    virtual void visit(cmp_ins &node) = 0;
    virtual void visit(logic_ins &node) = 0;
};


} // namespace ir





#endif