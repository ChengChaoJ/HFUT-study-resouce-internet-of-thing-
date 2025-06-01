#include "ir_printer.hpp"
#include<cassert>

void ir::IrPrinter::visit(ir_reg &node)
{
    out << this->mapping[node.type] << " ";
    out << "%" << "r" <<node.id;
}//例 float %r123   i32 %r456  寄存器的类型 %r寄存器的id

void ir::IrPrinter::visit(ir_constant &node)
{
    out << mapping[node.type] << " ";
    auto value = node.init_val.value();//获取init_val的值 类型为std::variant<int, float>
    if(std::holds_alternative<int>(value)){//holds_alternative来检查value是否持有int类型的值
        out << std::get<int>(value);//从variant中提取并打印出整数值
    }else{
        out << std::get<float>(value);
    }
}//例 i32 42   float 3.14

void ir::IrPrinter::visit(ir_basicblock &node)
{
    if(!node.instructions.size()) return;//指令列表为空
    out << node.name << ":" << std::endl;
    for(auto & inst : node.instructions)//遍历指令列表中的所有指令
        inst->accept(*this);//visit每一个指令
}
/*例
BB1:  
i32 %1 = constant 42
float %2 = constant 3.14
%3 = add %1, %2
br label %4
*/
void ir::IrPrinter::visit(ir_module& node){
    for(auto & [name,func] : node.usrfuncs){//遍历指令列表中的所有用户定义函数
        func->accept(*this);//visit每一个函数
    }
}

void ir::IrPrinter::visit(ir_userfunc &node)
{    
    out << "define" << " " << "dso_local" << " " << mapping[node.rettype]
    <<  " " << "@" << node.name <<"()"<< " " <<"{" << std::endl;
    for(auto & bb : node.bbs)//遍历所有基本块
        bb->accept(*this);//visit每一个基本块
    out << "}" << std::endl;
}
// 例
//  define dso_local i32 @my_add() {
//     %1 = constant 10
//     %2 = constant 20
//     %3 = add %1, %2
//     ret %3
// }

void ir::IrPrinter::visit(store &node)//存储指令
{
    out << "\t" << "store ";
    node.value->accept(*this);//visit值
    out << "," << " ";
    node.addr->accept(*this);//visit地址
    out << std::endl;
}//例  	store i32 %1, i32* %2

void ir::IrPrinter::visit(jump &node)//跳转语句
{
    out<<"\t"<<"br "<<"label "<<"%"<<node.target->name<<std::endl;
}//    br label %BB1

void ir::IrPrinter::visit(br &node)//条件分支
{
    out << "\t" << "br" << " ";
    out << "i1 ";
    out << this->get_value(node.cond);//get_value函数获取并打印条件表达式的值 %cond
    out << ",label ";
    out << "%" <<  node.target_true->name;
    out << ",label ";
    out << "%" <<  node.target_false->name;
    out << std::endl;
}//    br i1 %cond,label %BB1, label %BB2

void ir::IrPrinter::visit(ret &node)//返回指令
{
    if(!node.has_return_value)
        out << '\t' << "ret" << " "<< "void" << std::endl;
    else{
        out << "\t" << "ret" << " ";
        node.value->accept(*this);
        out << std::endl;
    }
}//    ret void  或     ret i32 %some_value

void ir::IrPrinter::visit(load &node)//加载指令
{
    out << "\t" << "%r"<< node.dst->id <<" = "<< "load " << mapping[node.dst->type] << "," << " ";
    node.addr->accept(*this);//打印地址
    out << std::endl; 
}//    %r1 = load i32, %ptr

void ir::IrPrinter::visit(alloc &node)//内存分配指令
{
    out<<"\t"<<"%";
    out<<"r"<< node.var->addr->id;
    out<<" = "<<"alloca ";
    out<<base_type[node.var->addr->type]<<", align 4";
    out<<std::endl;
}//    %r1 = alloca i32, align 4

void ir::IrPrinter::visit(phi &node)
{
    out << "\t";
    out << "%r" << node.dst->id << " = " <<  "phi" << " " << mapping[node.dst->type] << " ";
    for(int i = 0 ; i < node.uses.size(); ++i){//遍历Phi函数的输入列表node.uses
        auto & [a,b] = node.uses[i];
        out << "[" << " ";
        out << this->get_value(a);//%v1
        out << "," ;
        out << "%" << b->name << " " << "]" ;
        if(i != node.uses.size() - 1) out << ", ";//之后还有更多的输入对打印逗号
    }
    out << std::endl;
}//    %r3 = phi i32 [ %v1, %BB1 ], [ %v2, %BB2 ]

void ir::IrPrinter::visit(unary_op_ins &node)
{
    out<<"\t"<<"%";
    auto dst_r = std::dynamic_pointer_cast<ir::ir_reg>(node.dst);
    out<<"r"<<dst_r->id<<" = ";
    out<< mapping[node.op] << " ";
    if(node.op != unaryop::op_not){
        out<<"nsw ";
    }
    out << "i32 0, " << this->get_value(node.src) << std::endl;
}

void ir::IrPrinter::visit(binary_op_ins &node)
{
    out<<"\t"<<"%";
    auto dst_r = std::dynamic_pointer_cast<ir::ir_reg>(node.dst);
    out<<"r"<<dst_r->id<<" = ";
    out<<mapping[node.op] << " ";
    if(node.op != binop::divide){
        out<<"nsw ";
    }
    node.src1->accept(*this);
    out<<", ";
    out << this->get_value(node.src2);
    out<<std::endl;
}//    %r1 = add nsw i32 %src1, %src2

void ir::IrPrinter::visit(cmp_ins &node)
{
    out << "\t" << "%r" << node.dst->id << " = icmp "<< mapping[node.op] << " ";
    auto src1_r = std::dynamic_pointer_cast<ir::ir_constant>(node.src1);
    if(src1_r){
        out << mapping[src1_r->type] << " ";
        auto value = src1_r->init_val.value();
        if(std::holds_alternative<int>(value)){
            out << std::get<int>(value);
        }else{
            out << std::get<float>(value);
        }
    }else{
        auto aa = std::dynamic_pointer_cast<ir::ir_reg>(node.src1);
        out << mapping[aa->type] << " "<< "%r" << aa->id;
    }
    out << ",";
    out << this->get_value(node.src2);
    out<<std::endl;
}//	%r3 = icmp eq i32 %r1, %r2

void ir::IrPrinter::visit(logic_ins &node)
{
    out << "\t" << "%r" << this->get_value(node.dst) << " =  "<< mapping[node.op] << " ";
    auto src1_r = std::dynamic_pointer_cast<ir::ir_constant>(node.src1);
    if(src1_r){
        out << mapping[src1_r->type] << " ";
        auto value = src1_r->init_val.value();
        if(std::holds_alternative<int>(value)){
            out << std::get<int>(value);
        }else{
            out << std::get<float>(value);
        }
    }else{
        auto aa = std::dynamic_pointer_cast<ir::ir_reg>(node.src1);
        out << mapping[aa->type] << " "<< "%r" << aa->id;
    }
    out << ",";
    out << this->get_value(node.src2);
    out<<std::endl;
}

std::string ir::IrPrinter::get_value(const ptr<ir::ir_value> &val)
{
    std::string ans;
    auto aa = std::dynamic_pointer_cast<ir::ir_constant>(val);
    if(aa){
        auto value = aa->init_val.value();
        if(std::holds_alternative<int>(value)){   
           ans += std::to_string(std::get<int>(value));
        }else{
            ans += std::to_string(std::get<float>(value));
        }
    }else{
        auto aa = std::dynamic_pointer_cast<ir::ir_reg>(val);
        ans += ("%r" + std::to_string(aa->id));
    }
    return ans;
}
