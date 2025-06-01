#ifndef IR_HPP
#define IR_HPP

#include "parser/SyntaxTree.hpp"
#include <variant>
#include <list>
#include <optional>
#include <unordered_map>
#include <queue>
#include <algorithm>

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
class ir_visitor;
class IrPrinter;
class ir_func;
class ir_userfunc;
class ir_libfunc;

class ir_value;
class ir_reg;
class ir_constant;

class ir_basicblock;
class ir_memobj;

class alloc;
class binary_op_ins;
class br;
class store;
class phi;
class jump;
class load;
class ret;
class cmp_ins;
class control_ins;

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
    int id;                                                     //virtual register id
    vartype type;                                               //int or float (extension)
    int size;                                                   //some byte
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
public:
    ir_constant(std::optional<std::variant<int,float>> init_val) : init_val(init_val) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class jumpList : public ir_value {
    friend IrBuilder;
    std::vector<ptr<ir_basicblock>*> trueList;
    std::vector<ptr<ir_basicblock>*> falseList;
public:
    virtual void accept(ir_visitor& visitor);
    virtual void print(std::ostream & out = std::cout);
};

class ir_memobj : public printable {
    friend IrBuilder;
    friend IrPrinter;
protected:
    std::string name;
    int size;
    std::shared_ptr<ir_reg> addr;
public:
    ptr<ir_reg> get_addr() { return this->addr;};
    int get_size() {return this->size;}
    ir_memobj(std::string name , ptr<ir_reg> addr, int size) : name(name) , addr(addr) , size(size) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class ir_func_arg : public ir_memobj {
    int id;
};

class ir_scope : public printable {
    friend LoongArch::ProgramBuilder;
    friend ir_userfunc;
    std::list<std::shared_ptr<ir_memobj>> ir_objs;
public:
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
};

class ir_instr : public printable {
public:
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
    virtual std::vector<ptr<ir::ir_value>> use_reg() = 0;
    virtual std::vector<ptr<ir::ir_value>> def_reg() = 0;     
};

class ir_basicblock : public printable {
    friend IrPrinter;   
    friend LoongArch::ProgramBuilder; 
    std::string name;
    int id;
    std::list<std::shared_ptr<ir_instr>> instructions;
public:
    ir_basicblock(int id) : id(id) { name = "bb"+std::to_string(id); };
    void push_back(ptr<ir_instr> inst);
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    bool operator<(ir::ir_basicblock & rhs){return this->id < rhs.id;}
    std::shared_ptr<ir_instr> getLastInst();
    std::shared_ptr<ir_instr> getFirstInst();
    std::string getName();
    void for_each(std::function<void(std::shared_ptr<ir::ir_instr> inst)> f,bool isReverse);
};


class ir_func : public printable {
protected:
    std::string name;
    vartype rettype;
    std::unordered_map<int,ir_func_arg> args;
public:
    ir_func(std::string name) : name(name) {}
    bool set_retype(vartype rettype);
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
};

class ir_module : public printable {
    friend IrPrinter;
protected:
    std::unique_ptr<ir_scope> scope;

public:    
    std::list<std::pair<std::string,std::shared_ptr<ir_userfunc>>> usrfuncs;
    std::list<std::pair<std::string,std::shared_ptr<ir_libfunc>>> libfuncs;
    ir_module(){
        this->scope = std::make_unique<ir_scope>();
    }
    ptr<ir_userfunc> new_func(std::string name);
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual void reg_allocate();
};

//below is func
class ir_libfunc : public ir_func {

public:
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
};

class ir_userfunc : public ir_func , public std::enable_shared_from_this<ir_userfunc> {
public:
    friend IrPrinter;
    friend IrBuilder;
    friend LoongArch::ProgramBuilder;
private:
    std::unique_ptr<ir_scope> scope;
    std::list<std::shared_ptr<ir_basicblock>> bbs;
    int max_reg, max_bb;
    ptr<ir_basicblock> entry;
    std::unordered_map<int,std::shared_ptr<ir::ir_instr>> revInstLineNumber;
    std::unordered_map<std::shared_ptr<ir::ir_instr>,int> instLineNumber;
    std::unordered_map<std::shared_ptr<ir::ir_reg>,int> regAllocateOut;
public:
    ir_userfunc(std::string name); 
    ptr<ir_memobj> new_obj(std::string name);
    ptr<ir_reg> new_reg(vartype type);//自动创建序号递增的寄存器
    ptr<ir_basicblock> new_block();//创建BB
    virtual void accept(ir_visitor& visitor);
    virtual void print(std::ostream & out = std::cout) override;
    std::unordered_map<ptr<ir::ir_value>,Pass::LiveInterval> GetLiveInterval();
    std::vector<std::shared_ptr<ir_basicblock>> GetLinerSequence();
    virtual void reg_allocate();
};

//below is instruction
class reg_write_ins : public ir_instr {

    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override;
};

class control_ins : public ir_instr {
    virtual void accept(ir_visitor& visitor) = 0;
    virtual void print(std::ostream & out = std::cout) = 0;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override;
};

class store : public ir_instr {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_value> addr;
    ptr<ir_value> value;
public:
    store(ptr<ir_value> addr,ptr<ir_value> value):addr(addr),value(value){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class jump : public control_ins {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_basicblock> target;
public:
    jump(ptr<ir_basicblock> target):target(target){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
    std::shared_ptr<ir::ir_basicblock> getTarget();
};

class br : public control_ins {
    friend IrBuilder;
    friend IrPrinter;
    ptr<ir_value> cond;
    ptr<ir_basicblock>target_true, target_false;
public:
    ptr<ir_basicblock> get_target_true() {return this->target_true;};
    ptr<ir_basicblock> get_target_false() { return this->target_false;};
    void set_target_true(ptr<ir_basicblock> bb){this->target_true=bb;}
    void set_target_false(ptr<ir_basicblock> bb){this->target_false=bb;}
    br(ptr<ir_value> cond,ptr<ir_basicblock> target_true,ptr<ir_basicblock> target_false):cond(cond), target_true(target_true),target_false(target_false){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class ret : public control_ins {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_value> value;
    bool has_return_value;
public:
    ret(ptr<ir_value>  value,bool has_return_value):value(value),has_return_value(has_return_value){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class load : public reg_write_ins {
    friend IrPrinter;
    friend LoongArch::ProgramBuilder;
    ptr<ir_reg> dst;
    ptr<ir_value>  addr;
public:
    load(ptr<ir_reg> dst,ptr<ir_value> addr): dst(dst),addr(addr){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class alloc : public reg_write_ins {
    friend IrPrinter;
    ptr<ir_memobj> var;
public:
    alloc(ptr<ir_memobj>  var):var(var){}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class phi : public reg_write_ins {
public:
    friend IrPrinter;    
    friend class IrBuilder;
    std::vector<std::pair<ptr<ir_value>,ptr<ir_basicblock>>> uses;
    ptr<ir_reg> dst;
public:
    phi(ptr<ir_reg> dst): dst(dst) {}
    virtual void accept(ir_visitor& visitor) override final;
    virtual void print(std::ostream & out = std::cout) override final;
    virtual std::vector<ptr<ir::ir_value>> use_reg() override final;
    virtual std::vector<ptr<ir::ir_value>> def_reg() override final;
};

class unary_op_ins : public reg_write_ins {
    friend IrPrinter;    
    std::shared_ptr<ir_value> dst, src;
    unaryop op;
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
    std::shared_ptr<ir_value> src1,src2;
    std::shared_ptr<ir_reg> dst;
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