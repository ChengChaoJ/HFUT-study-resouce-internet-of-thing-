#pragma once

#include <array>
#include <cstdint>
#include <ostream>

namespace LoongArch {

constexpr int RegCount = 32; //记录寄存器的数量
constexpr int zero = 0, ra = 1, tp = 2, sp = 3, x = 21, fp = 22;  //记录不同寄存器的编号

constexpr int arch_bit_width = 16; //寄存器位宽

enum RegisterUsage { caller_save, callee_save, special };
enum Rtype { INT };
enum Compare { Eq, Ne, Lt, Le, Gt, Ge };



constexpr RegisterUsage REGISTER_USAGE[RegCount] = {//预设的寄存器使用方式
    special,     callee_save, special,     caller_save,
    //zero ra tp sp

    caller_save, caller_save, caller_save, caller_save,
    caller_save, caller_save, caller_save, caller_save,  // a0-a7
    
    caller_save, caller_save, caller_save, caller_save,
    caller_save, caller_save, caller_save, caller_save, //t0 - t8
    caller_save,
    
    special,

    callee_save,

    callee_save, callee_save, callee_save, callee_save,
    callee_save, callee_save, callee_save, callee_save, //s0 - s8
    callee_save,

};


constexpr bool integer_allocable(int reg_id) { //可以分配的寄存器是caller_save和callee_save类型的寄存器
  return REGISTER_USAGE[reg_id] == caller_save ||
         REGISTER_USAGE[reg_id] == callee_save;
}

constexpr int ALLOCABLE_REGISTER_COUNT = []() constexpr { //计算可以分配的寄存器的个数
  int cnt = 0;
  for (int i = 0; i < RegCount; ++i)
    if (integer_allocable(i)) ++cnt;
  return cnt;
}
();

constexpr std::array<int, ALLOCABLE_REGISTER_COUNT> ALLOCABLE_REGISTERS =  //将能分配的寄存器保存在一个数组里面
    []() constexpr {
  std::array<int, ALLOCABLE_REGISTER_COUNT> ret_array{};
  int cnt = 0;
  for (int i = 0; i < RegCount; ++i)
    if (integer_allocable(i)) ret_array[cnt++] = i;
  return ret_array;
}
();


constexpr int ARGUMENT_REGISTER_COUNT = 8;  //用于参数传递寄存器的数量

constexpr int ARGUMENT_REGISTERS[ARGUMENT_REGISTER_COUNT] = {4, 5, 6, 7,
                                                             8, 9, 10, 11};  //保存寄存器编号


constexpr int32_t IMM12_L = -2048, IMM12_R = 2047;  //12位立即数的取值范围

constexpr bool is_imm12(int32_t value) {  //判断是否在范围之内
  return value >= IMM12_L && value <= IMM12_R;
}



struct Reg {
  int id;
  Rtype type;
  int offset;
  Reg(int _id = -1,Rtype _type = INT) : id(_id),type(_type),offset(0) {}
  bool is_machine() const { return id < RegCount; }
  bool is_virtual() const { return id >= RegCount; }//这个就是编号大于可用的物理寄存器的东西
  bool is_float() const {return false;}
  bool operator<(const Reg &rhs) const { return id < rhs.id || (id == rhs.id && type < rhs.type); }
  bool operator==(const Reg &rhs) const { return ( id == rhs.id) && (is_float() == rhs.is_float()); }
  bool operator>(const Reg &rhs) const { return id > rhs.id || (id == rhs.id && type > rhs.type); }
  bool operator!=(const Reg &rhs) const { return (id != rhs.id) || (is_float() == rhs.is_float()); }
};



constexpr Compare logical_not(Compare c) {  //逻辑运算
  switch (c) {
    case Eq:
      return Ne;
    case Ne:
      return Eq;
    case Lt:
      return Ge;
    case Le:
      return Gt;
    case Gt:
      return Le;
    case Ge:
      return Lt;
  }
}

inline std::ostream &operator<<(std::ostream &os, Compare c) {  //输出运算符
  switch (c) {
    case Eq:
      os << "eq";
      break;
    case Ne:
      os << "ne";
      break;
    case Lt:
      os << "lt";
      break;
    case Le:
      os << "le";
      break;
    case Gt:
      os << "gt";
      break;
    case Ge:
      os << "ge";
      break;
  }
  return os;
}

}  // namespace archLA