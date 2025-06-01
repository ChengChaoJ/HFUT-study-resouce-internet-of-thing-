#pragma once
#include<unordered_map>
#include <queue>
#include <set>
#include <utility>
#include <vector>
#include "ir/ir.hpp"


namespace LoongArch {

struct Reg;
struct Function;

// no coalescing
class ColoringAllocator {
  std::shared_ptr<ir::ir_userfunc> func;
  std::unordered_map<std::shared_ptr<ir::ir_reg> ,std::vector<std::shared_ptr<ir::ir_reg>> > conflictGraph;
  std::unordered_map<std::shared_ptr<ir::ir_reg>,Pass::LiveInterval> mappingToInterval;
  std::unordered_map<std::shared_ptr<ir::ir_reg>,int> mappingToReg;
  void SimplifyGraph();
  void Spill();
  void BuildConflictGraph();
  std::unordered_map<std::shared_ptr<ir::ir_reg>,int> getAllocate();
  // void ExpireOldInterval(vector<int> & liveseq, Pass::LiveIntervali);
 public:
  ColoringAllocator(std::shared_ptr<ir::ir_userfunc> _func);
  std::unordered_map<std::shared_ptr<ir::ir_reg>,int> run();
};

}  // namespace Archriscv