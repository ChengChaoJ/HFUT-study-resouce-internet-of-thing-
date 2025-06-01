#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include "ir.hpp"
#include <string>
#include <vector>
#include <map>
#include <cassert>
template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T>
using ptr_list = std::vector<ptr<T>>;

namespace symbol_table  //符号表
{


class FunctionTable //函数表
{
public:
    //添加函数
    bool push_func(std::string name, ptr<ir::ir_func> ir_fun);
    //查找函数
    ptr<ir::ir_func> find_func(std::string name);
private:
    std::map<std::string, ptr<ir::ir_func>> table;
};

class Scope //作用域
{
public:
    //进入一个新的作用域
    void enter();
    //退出当前作用域
    void exit();
    //放入变量
    bool push_var(std::string name, ptr<ir::ir_memobj> var);
    //查找变量
    ptr<ir::ir_memobj> find_var(std::string name);
private:
    std::vector<std::map<std::string, ptr<ir::ir_memobj>>> layers;//多个作用域的层叠结构
    // 每个 std::map 代表一个单独的作用域 存储多个{名字:变量}

};

}



#endif