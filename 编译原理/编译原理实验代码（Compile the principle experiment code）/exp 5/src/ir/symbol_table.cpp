#include "ir/symbol_table.hpp"

bool symbol_table::FunctionTable::push_func(std::string name, ptr<ir::ir_func> ir_fun)
{
    auto result=this->table.insert({name,ir_fun});
    assert(result.second);//没通过说明有同名的函数，SYSY不支持函数重载
    return result.second;
}
ptr<ir::ir_func> symbol_table::FunctionTable::find_func(std::string name)
{
    auto item=this->table.find(name);
    assert(item!=this->table.end());//没找到函数,出错
    return item->second;
    // TODO: insert return statement here
}

void symbol_table::Scope::enter()
{
    layers.push_back({});

}

void symbol_table::Scope::exit()
{
    layers.pop_back();
}

bool symbol_table::Scope::push_var(std::string name, ptr<ir::ir_memobj> var)
{
    auto result =layers[layers.size()-1].insert({name,var});
    assert(result.second);//不通过说明重定义了
    return result.second;
}

ptr<ir::ir_memobj> symbol_table::Scope::find_var(std::string name)
{
    for(auto s=layers.rbegin();s!=layers.rend();s++)
    {
        auto iter=s->find(name);
        if(iter != s->end())
        {
            return iter->second;
        }
    }
     assert(0);//不通过说明没有定义变量
    return nullptr;
    // TODO: insert return statement here
}
