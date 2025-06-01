#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include "ir/ir.hpp"
#include <string>
#include <vector>
#include <map>
#include <cassert>
template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T>
using ptr_list = std::vector<ptr<T>>;

namespace symbol_table
{


class FunctionTable
{
public:
    bool push_func(std::string name, ptr<ir::ir_func> ir_fun);
    ptr<ir::ir_func> find_func(std::string name);//查找函数
private:
    std::map<std::string, ptr<ir::ir_func>> table;
};

class Scope
{
public:
    void enter();//进入一个新的作用域
    void exit();//退出当前作用域
    bool push_var(std::string name, ptr<ir::ir_memobj> var);//放入变量
    ptr<ir::ir_memobj> find_var(std::string name);//查找变量
private:
    std::vector<std::map<std::string, ptr<ir::ir_memobj>>> layers;//层  名字:变量

};

}



#endif