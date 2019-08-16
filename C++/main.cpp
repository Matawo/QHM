#include <iostream>
#include <memory>
#include "Name.h"
#include "Name.cpp"
#include "Graph.h"

int main(){
    for(int i = 0; i<10000; i++) {
        SimpleName n1 = SimpleName(1,{});
        SimpleName n2 = SimpleName(2,{});
        SimpleName n3 = SimpleName(3,{});
        auto a = shared_ptr<SimpleName>(new SimpleName(12,{true, false, true, false}));
        auto b = shared_ptr<SimpleName>(new SimpleName(12, {true, false, true, true}));
        ComposedName c = ComposedName(a,b);
        ComposedName d = ComposedName(c,b);
        Name* left = d.get_left();
        Name* right = d.get_right();
        Name** names = new Name*[3];
        names[0] = &n1;
        names[1] = &n2;
        names[2] = &n3;
        vector<bool> particles = {true,false,true, true,false,false};
        auto test = Graph(3, std::complex<double>(1.0, 0.0), particles , names);
        std::cout << test.to_string() << "\n";
        std::cout << test.shift().to_string();
    }
    return 0;
}