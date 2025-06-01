#include "symbol_table.hpp"

bool symbol_table::FunctionTable::push_func(std::string name, ptr<ir::ir_func> ir_fun)
{
    auto result=this->table.insert({name,ir_fun});//pair类型，result.first 是指向新插入元素的迭代器，result.second 是一个布尔值，表示插入操作是否成功
    assert(result.second);//没通过说明有同名的函数，SYSY不支持函数重载  //assert:如果条件不满足，程序将终止
    return result.second;
}
ptr<ir::ir_func> symbol_table::FunctionTable::find_func(std::string name)
{
    auto item=this->table.find(name);//没找到则返回map结束位置的迭代器
    assert(item!=this->table.end());//没找到函数,出错
    return item->second;
    // TODO: insert return statement here
}

void symbol_table::Scope::enter()
{
    layers.push_back({});//添加一个新的空映射 表示 进入一个新的作用域

}

void symbol_table::Scope::exit()
{
    layers.pop_back();
}

bool symbol_table::Scope::push_var(std::string name, ptr<ir::ir_memobj> var)
{
    auto result =layers[layers.size()-1].insert({name,var});//layers[layers.size()-1]是最后一个map
    //result.first 是指向新插入元素的迭代器
    assert(result.second);//不通过说明重定义了
    return result.second;
}

ptr<ir::ir_memobj> symbol_table::Scope::find_var(std::string name)
{
    for(auto s=layers.rbegin();s!=layers.rend();s++)//遍历每一个map(作用域)
    {//从layers中最后一个元素（最内层的作用域）开始向前遍历到第一个（最外层的作用域） 
     //因为内层作用域的变量会遮蔽外层作用域的同名变量
        auto iter=s->find(name);//在当前作用域查找键为name的元素
        if(iter != s->end())
        {
            return iter->second;
        }
    }
    assert(0);//不通过说明没有定义变量
    return nullptr;
    // TODO: insert return statement here
}
