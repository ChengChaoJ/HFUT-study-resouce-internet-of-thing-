#include "ir/ir.hpp"
#include <cassert>



namespace Pass
{


class pass_base {
public:
  pass_base(std::shared_ptr<ir::ir_module> unit): c(unit) { };
  void set_name(std::string name) {this->name_ = name;}
  std::string get_name() {return name_;}
  virtual void run() = 0; 
  virtual void run(int n) {}
protected:
    std::shared_ptr<ir::ir_module> c;
    std::string name_;
};

class mem2reg_pass : public pass_base {
public:
  mem2reg_pass(std::shared_ptr<ir::ir_module> c) : pass_base(c) {};
  ~mem2reg_pass(){};
  virtual void run() override;
};

class PassManager {
private:
  std::shared_ptr<ir::ir_module> c;
  
  std::vector<std::shared_ptr<pass_base>> mypass;
  std::vector<bool> print_ir_opt;
public:
  PassManager(std::shared_ptr<ir::ir_module>  unit) : c(unit) {};
  void add_pass(std::string pass_name,bool is_print = false) {
    if (pass_name == "mem2reg") {
      mypass.emplace_back(std::move(new mem2reg_pass(c)));
      mypass.back()->set_name("mem2reg");
      print_ir_opt.emplace_back(is_print);
    }
    else assert(0);
  }
  void run_pass() {
    for (int i = 0; i < mypass.size(); i++) {
      mypass[i]->run();
    }
  }
  ~PassManager(){};
};
} // namespace Pass


