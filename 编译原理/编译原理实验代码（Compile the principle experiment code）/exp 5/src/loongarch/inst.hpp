#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

#include "arch.hpp"
#include "code_gen.hpp"

namespace LoongArch {
class ColoringAllocator;
inline std::ostream &operator<<(std::ostream &os, const Reg &reg) {
  os << '$' << 'r' << reg.id << " ";
  return os;
}

class AsmContext;

struct Inst {
  virtual ~Inst() = default;

  virtual std::vector<Reg> def_reg() { return {}; }
  virtual std::vector<Reg> use_reg() { return {}; }
  virtual std::vector<Reg *> regs() { return {}; }
  virtual bool side_effect() {
    return false;
  } 
  virtual void gen_asm(std::ostream &out) = 0;  //生成汇编
  virtual void print(std::ostream &out) { gen_asm(out); } //打印

  template <class T> T *as() {
    return dynamic_cast<T *>(this);
  }
  void update_live(std::set<Reg> &live) {
    for (Reg i : def_reg())
      if (i.is_virtual() || integer_allocable(i.id)) live.erase(i);
    for (Reg i : use_reg())
      if (i.is_virtual() || integer_allocable(i.id)) live.insert(i);
  }
  bool def(Reg reg) {
    for (Reg r : def_reg())
      if (r == reg) return true;
    return false;
  }
  bool use(Reg reg) {
    for (Reg r : use_reg())
      if (r == reg) return true;
    return false;
  }
  bool relate(Reg reg) { return def(reg) || use(reg); }
  void replace_reg(Reg before, Reg after) {
    for (Reg *i : regs())
      if ((*i).id == before.id) (*i) = after;
  }
};

//modified
struct RegRegInst : Inst {      //R-Type Instruction like add r1, r2 ——> r3
  enum Type {
    add_w,add_d,
    sub_w,sub_d,
    mul_w,
    div_w,

    andw,orw,
    slt
  } op;
  Reg dst, lhs, rhs;
  RegRegInst(Type _op, Reg _dst, Reg _lhs, Reg _rhs)
      :  dst(_dst), lhs(_lhs), rhs(_rhs), op(_op) 
      {
      }

  virtual std::vector<Reg> def_reg() override { return {dst}; }
  virtual std::vector<Reg> use_reg() override { return {lhs, rhs}; }
  virtual std::vector<Reg *> regs() override { return {&dst, &lhs, &rhs}; }
  virtual void gen_asm(std::ostream &out) override {
    static const std::map<Type, std::string> asm_name
    {
        //Integer
        {add_w, "add.w"}, {sub_w, "sub.w"}, {mul_w, "mul.w"},  {div_w, "div.w"}, {andw, "and"}, {orw, "or"},
        {slt, "slt"}
    };
    out << asm_name.find(op)->second << ' ' << dst << ", " << lhs << ", " << rhs<< '\n';
  }
};

struct RegImmInst : Inst {
  enum Type { 
    addi_d , addi_w , slli, srli, srai, andi, ori, slti
  } op;
  Reg dst, lhs;
  int32_t rhs;
  RegImmInst(Type _op, Reg _dst, Reg _lhs, int32_t _rhs)
      : op(_op), dst(_dst), lhs(_lhs), rhs(_rhs) {
  }

  virtual std::vector<Reg> def_reg() override { return {dst}; }
  virtual std::vector<Reg> use_reg() override { return {lhs}; }
  virtual std::vector<Reg *> regs() override { return {&dst, &lhs}; }
  virtual void gen_asm(std::ostream &out) override {
    static const std::map<Type, std::string> asm_name 
    {
        {addi_d, "addi.d"},{addi_w, "addi.w"}, {andi, "andi"}, {ori, "ori"},
        {slti, "slti"}
    };
      out << asm_name.find(op)->second << ' ' << dst << ", " << lhs << ", " << rhs<< '\n';
      
  }
};

struct LoadImm : Inst { //用于将立即数加载到寄存器的汇编代码。
  Reg dst;
  int32_t value;
  LoadImm(Reg _dst, int32_t _value) : dst(_dst), value(_value) {}

  virtual std::vector<Reg> def_reg() override { return {dst}; }
  virtual std::vector<Reg *> regs() override { return {&dst}; }
  virtual void gen_asm(std::ostream &out) override {
    out << "ori " << dst << ", " << "$r0" << ',' << value << '\n';
  }
};

struct Jump : Inst {  //用于生成无条件跳转指令的汇编代码。
  LoongArch::Block *target;
  Jump(Block *_target) : target(_target) { target->label_used = true; }

  virtual bool side_effect() override { return true; }
  virtual void gen_asm(std::ostream &out) override {
    out << "b " << target->name << '\n';
  }
};

struct jr : Inst { //用于生成函数返回指令的汇编代码。
  bool has_return_value;
  jr(bool _has_return_value) : has_return_value(_has_return_value) {}

  virtual std::vector<Reg> use_reg() override {
    if (has_return_value)
      return {Reg{ra}};
    else
      return {};
  }
  virtual bool side_effect() override { return true; }
  virtual void gen_asm(std::ostream &out) override {
    // ctx->epilogue(out);
    out << "jr\t" << '$' << "ra" << '\n'; 
  }
  virtual void print(std::ostream &out) override { out << "jr\t" << '$' << "ra"; out << "ret\n"; }
};


struct st : Inst {
  enum Type {
    st_d,
    st_w,
  } op;
  Reg src, base;
  int32_t offset;
  st(Reg _src, Reg _base, int32_t _offset, Type _op = st_d)
      : op(_op), src(_src), base(_base), offset(_offset) {
    // assert(is_imm12(offset));
  }

  virtual std::vector<Reg> use_reg() override { return {src, base}; }
  virtual std::vector<Reg *> regs() override { return {&src, &base}; }
  virtual bool side_effect() override { return true; }
  virtual void gen_asm(std::ostream &out) override {
    static const std::map<Type, std::string> asm_name {
        {st_d, "st.d"}, {st_w, "st.w"}
    };
      out << asm_name.find(op)->second << ' ' << src << ", " << base <<  "," << offset <<"\n";
  }
};

struct ld : Inst {
  enum Type {
    ld_d,
    ld_w,
  } op;
  Reg src, base;
  int32_t offset;
  ld(Reg _src, Reg _base, int32_t _offset, Type _op = ld_d)
      : op(_op), src(_src), base(_base), offset(_offset) {
    // assert(is_imm12(offset));
  }

  virtual std::vector<Reg> use_reg() override { return {src, base}; }
  virtual std::vector<Reg *> regs() override { return {&src, &base}; }
  virtual bool side_effect() override { return true; }
  virtual void gen_asm(std::ostream &out) override {
    static const std::map<Type, std::string> asm_name {
        {ld_d, "ld.d"}, {ld_w, "ld.w"}
    };
      out << asm_name.find(op)->second << ' ' << src << ", "  << base << "," << this->offset << "\n";
  }
};


}  // namespace Archriscv
