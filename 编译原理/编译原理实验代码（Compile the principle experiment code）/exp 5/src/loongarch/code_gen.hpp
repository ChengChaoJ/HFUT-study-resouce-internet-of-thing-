#pragma once

#include <functional>
#include <map>
#include <sstream>
#include <vector>
#include <memory>
#include <set>
#include <iostream>

#include "ir/ir.hpp"
#include "loongarch/arch.hpp"
#include "loongarch/register_allocator.hpp"

namespace LoongArch {


class Program;
class Function;
class Block;
class ProgramBuilder;
class IrMapping;
struct Inst;
struct Jump;
struct RegImmInst;


class Program {
    friend LoongArch::ProgramBuilder;
protected:
    std::stringstream asm_code;
    std::vector<std::shared_ptr<Function>> functions;
    int block_n;
public:
    Program();
    void get_asm(std::ostream &out);
};

class Function : public std::enable_shared_from_this<Function> {
    friend LoongArch::ProgramBuilder;
protected:
    int regn; 
    std::string name;
    std::shared_ptr<LoongArch::Block> entry,ret_block;
    std::vector<std::shared_ptr<LoongArch::Block>> blocks;
    std::stringstream asm_code;
    int stack_size;
    int cur_pos;
    int bitalign(int value, int align);
public:
    Function(std::string name);
    void get_asm(std::ostream& out);
};

class Block {
    friend LoongArch::ProgramBuilder;
    friend LoongArch::Jump;
protected:
    bool label_used;
    std::string name;
    std::list<std::shared_ptr<LoongArch::Inst>> instructions;
    std::vector<std::shared_ptr<LoongArch::Block>> in_edge, out_edge;
    std::set<LoongArch::Reg> liveIn,liveOut,def,use;
public:
    Block(std::string name);
    void insert_before_jump(std::shared_ptr<Inst> inst);
    void get_asm(std::ostream &out);

};

class IrMapping{
    friend LoongArch::ProgramBuilder;
    int regn;
protected:
    std::unordered_map<std::shared_ptr<ir::ir_basicblock>,std::shared_ptr<LoongArch::Block>> blockmapping;
    std::unordered_map<std::shared_ptr<LoongArch::Block>,std::shared_ptr<ir::ir_basicblock>> rev_blockmapping;
    std::unordered_map<int,LoongArch::Reg> reg_mapping;
    std::unordered_map<std::shared_ptr<ir::ir_memobj>,int> objoffset_mapping;
    std::unordered_map<std::shared_ptr<ir::ir_reg>,std::shared_ptr<ir::ir_memobj>> addrMemObj;
    std::unordered_map<std::shared_ptr<ir::ir_memobj>,std::shared_ptr<ir::ir_reg>> revAddMemObj;

public:
    Reg new_reg();
    Reg transfer_reg(ir::ir_reg irReg);
    IrMapping();
};



} // namespace ArchLA
