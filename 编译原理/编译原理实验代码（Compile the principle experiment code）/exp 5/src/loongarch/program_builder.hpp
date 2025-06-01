#include "ir/irbuilder.hpp"
#include "loongarch/inst.hpp"
// #include "loongarch/register_allocator.hpp"
namespace LoongArch {

class ProgramBuilder : public ir::ir_visitor {
public:
    virtual void visit(ir::ir_reg & node) override final;  
    virtual void visit(ir::ir_constant & node) override final;  
    virtual void visit(ir::ir_basicblock & node) override final;  
    virtual void visit(ir::ir_module & node) override final;  
    virtual void visit(ir::ir_userfunc & node) override final;  
    virtual void visit(ir::store & node) override final;  
    virtual void visit(ir::jump & node) override final;  
    virtual void visit(ir::br & node) override final;  
    virtual void visit(ir::ret & node) override final;  
    virtual void visit(ir::load & node) override final;  
    virtual void visit(ir::alloc & node) override final;  
    virtual void visit(ir::phi & node) override final;  
    virtual void visit(ir::unary_op_ins & node) override final ;
    virtual void visit(ir::binary_op_ins & node) override final;  
    virtual void visit(ir::cmp_ins& node) override final;  
    virtual void visit(ir::logic_ins & node) override final;
    std::shared_ptr<Program> prog;
protected:
    std::shared_ptr<Function> cur_func;
    std::shared_ptr<Block> cur_block, next_block;
    std::string func_name;     
    std::shared_ptr<IrMapping> cur_mapping;
};
} // namespace archLA

