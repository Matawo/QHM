#include <iostream>
#include "Name.h"

int main(){
    SimpleName a = SimpleName(12,{true, false, true, false});
    std::cout<<a.to_string()<<std::endl;
    return 0;
}