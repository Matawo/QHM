#include <iostream>
#include <memory>
#include "Name.h"
#include "Name.cpp"
#include "Graph.h"

int main(){
    //WARNING : dans la version actuelle, le premier nom doit contenir l'ancre default = 1.lll... (l==false)
    SimpleName* n1 = new SimpleName(1,{false});
    SimpleName* n2 = new SimpleName(2,{});
    SimpleName* n3 = new SimpleName(3,{});
    const std::complex<double> hadamard[4] = {std::complex<double>(1/sqrt(2),0.0),std::complex<double>(1/sqrt(2),0.0),
                                           std::complex<double>(1/sqrt(2),0.0),std::complex<double>(-1/sqrt(2),0.0)};
    SimpleName* n4 = new SimpleName(4,{});
    auto * a = new ComposedName(n1,n4);
    vector<bool> particles = {true,true,true,true,false,true};
    vector<Name*> names = {a,n2,n3};
    auto * test = new Graph(3, std::complex<double>(1.0, 0.0), particles , names);
    cout << test->to_string_amp() << "\n";
    auto result = test->interaction(hadamard);
    while (!result.empty()) {
        cout << result.back()<< " : " << result.back()->to_string_amp() <<"\n";
        auto* graph =  result.back();
        result.pop_back();
        delete graph;
    }
    delete test;
    return 0;
}