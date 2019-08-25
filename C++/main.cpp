#include <iostream>
#include <memory>
#include "Name.h"
#include "Name.cpp"
#include "Graph.h"

int main(){
    SimpleName* n1 = new SimpleName(1,{true});
    SimpleName* n2 = new SimpleName(2,{});
    SimpleName* n3 = new SimpleName(3,{});
    //SimpleName* n4 = new SimpleName(4,{});
    //auto * a = new ComposedName(n1,n4);
    vector<bool> particles = {true,false,true, true,false,false};
    vector<Name*> names = {n1,n2,n3};
    auto * test = new Graph(3, std::complex<double>(1.0, 0.0), particles , names);
    for(int i = 0; i< 10000; i++) {
        Graph* old = test;
        test = test->shift();
        delete old;
    }
    delete test;
    return 0;
}