#include "register_allocator.hpp"

void LoongArch::ColoringAllocator::SimplifyGraph() {}

void LoongArch::ColoringAllocator::Spill() {}

void LoongArch::ColoringAllocator::BuildConflictGraph() {
  //获得一个区间
  auto liveinterval = this->func->GetLiveInterval();
  // 获得一个所有寄存器的集合ir_reg
  std::set<std::shared_ptr<ir::ir_reg>> ir_set;
  for(auto &[a,b] : liveinterval){
    if(auto reg = std::dynamic_pointer_cast<ir::ir_reg>(a)){
      this->mappingToInterval[reg] = b;
      ir_set.insert(reg);
    }
  }
  //
  std::vector<std::shared_ptr<ir::ir_reg>> ir_regs(ir_set.begin(),ir_set.end());
  for(auto & reg : ir_regs){
    std::vector<std::shared_ptr<ir::ir_reg>> conflictWithreg;
    for(auto & reg_j : ir_set){
      if(this->mappingToInterval[reg].l > this->mappingToInterval[reg_j].l){
        if(this->mappingToInterval[reg].l < this->mappingToInterval[reg_j].r){
          conflictWithreg.push_back(reg_j);
        }
      }else{
        if(this->mappingToInterval[reg_j].l > this->mappingToInterval[reg].l){
          if(this->mappingToInterval[reg_j].l < this->mappingToInterval[reg].r){
            conflictWithreg.push_back(reg_j);
          }
        } 
      }
    }

    for(auto conflict_reg : conflictWithreg){
      this->conflictGraph[reg].push_back(conflict_reg);
    }
  }
}

std::unordered_map<std::shared_ptr<ir::ir_reg>, int>
LoongArch::ColoringAllocator::getAllocate() {
  std::queue<std::shared_ptr<ir::ir_reg>> q;
  std::unordered_map<std::shared_ptr<ir::ir_reg>,int> seen; 
  for(auto & p : this->conflictGraph){
    q.push(p.first);
    break;
  }
  for(auto & p : this->conflictGraph){
    seen[p.first] = false;
  }
  while(q.size()){
    //直到对所有的寄存器都分配完成
    auto front = q.front();
    q.pop();
    seen[front] = true;
    vector<int> unused(32,1);
    for(auto i : this->conflictGraph[front]){
      unused[this->mappingToReg[i]] = 0;
      if(!seen[i]){
        q.push(i);
      }
    }
    int wait_allocate = 0;
    for(int i = 13; i < 32; ++i){
      if(unused[i] == true){ //如果没有被邻接点使用
        wait_allocate = i;
        break;
      }
    }
    this->mappingToReg[front] = wait_allocate;
  }
  return this->mappingToReg;
}


LoongArch::ColoringAllocator::ColoringAllocator(
    std::shared_ptr<ir::ir_userfunc> _func) : func(_func)
{
  this->BuildConflictGraph();
  // auto reg_out = this->getAllocate();
  // cout << "使用一个很简单的寄存器分配的算法" << std::endl;
  // for(auto [a,b] : reg_out){
  //   a->print();
  //   cout << '\t';
  //   cout << b ;
  //   cout << std::endl;
  // }
  // cout << "分配结束了" << std::endl;
}

std::unordered_map<std::shared_ptr<ir::ir_reg>,int> LoongArch::ColoringAllocator::run() {
  return this->getAllocate();
}

