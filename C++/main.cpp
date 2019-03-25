#include <iostream>
#include "Name.h"
#include "Name.cpp"

int main(){
    SimpleName a = SimpleName(12,{true, false, true, false});
    SimpleName b = SimpleName(12, {true, false, true, true});
    ComposedName c = ComposedName(&a,&b);
    ComposedName d = ComposedName(&c,&b);
    Name* left = d.get_left();
    Name* right = d.get_right();
    std::cout<<a.to_string()<<std::endl;
    std::cout<<b.to_string()<<std::endl;
    std::cout<<c.to_string()<<std::endl;
    std::cout<<c.normalize()->to_string()<<std::endl;
    std::cout<<d.to_string()<<std::endl;
    std::cout<<left->to_string()<<std::endl;
    std::cout<<right->to_string()<<std::endl;
    std::cout<<a.is_brother(&b)<<std::endl;
    return 0;
}