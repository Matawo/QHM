#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include "Name.h"
#include "Name.cpp"
#include "Graph.h"
#include <unordered_map>

double error_margin = pow(10.0,-9);

complex<double> calculate_norm(vector<Graph*> superposition) {
    complex<double> result = std::complex<double>(0.0,0.0);
    for (auto &i : superposition) {
        result += i->getAmp() * i->getAmp();
    }
    return result;
}

double norm(std::complex<double> a) {
    return a.real() * a.real() - a.imag() * a.imag();
}

void merge_graphs(vector<Graph*>&superposition) {
    std::unordered_multimap<size_t, Graph*> sorted = unordered_multimap<size_t,Graph*>(0);
    while(!superposition.empty()){
        auto graph = superposition.back();
        size_t hash = graph->hash();
        auto range = sorted.equal_range(hash);
        bool exist = false;
        for(auto it = range.first; it!=range.second; it++) {
           if (graph->equals(it->second)) {
               it->second->setAmp(it->second->getAmp()+graph->getAmp());
               exist = true;
               if(graph->to_string() != it->second->to_string()) {
                   cout << graph->to_string_amp() << "     " <<it->second->to_string_amp() << "\n";
               }
               delete graph;
           }
        }
        if(not exist) {
            sorted.insert(std::pair<size_t,Graph*>(hash,graph));
        }
        superposition.pop_back();
    }
    for (auto &it : sorted) {
        cout << it.second->to_string() << "\n";
        if ( norm(it.second->getAmp())> error_margin) {
            superposition.push_back(it.second);
        } else {
            delete it.second;
        }
    }
}

void shift(vector<Graph*>&superposition) {
    vector<Graph*> result = vector<Graph*>(0);
    for(auto &graph:superposition) {
        result.push_back(graph->shift());
        delete graph;
    }
    superposition = result;
}

string to_string(vector<Graph*>superposition) {
    string result;
    for(auto &graph:superposition) {
        result += graph->to_string_amp() + "\n";
    }
    return result;
}


void interaction(vector<Graph*>&superposition,const std::complex<double> * unitary) {
    vector<Graph*> result = vector<Graph*>(0);
    while(!superposition.empty()) {
        auto graph = superposition.back();
        auto temp = graph->interaction(unitary);
        result.insert(result.end(),temp.begin(), temp.end());
        delete graph;
        superposition.pop_back();
    }
    superposition = result;
}

int main(){
    //WARNING : dans la version actuelle, le premier nom doit contenir l'ancre default = 1.lll... (l==false)
    SimpleName* n1 = new SimpleName(1,{});
    SimpleName* n2 = new SimpleName(2,{});
    SimpleName* n3 = new SimpleName(3,{});
    const std::complex<double> hadamard[4] = {std::complex<double>(1/sqrt(2),0.0),std::complex<double>(1/sqrt(2),0.0),
                                           std::complex<double>(1/sqrt(2),0.0),std::complex<double>(-1/sqrt(2),0.0)};
    //SimpleName* n4 = new SimpleName(4,{});
    //auto * a = new ComposedName(n1,n4);
    vector<bool> particles = {false,true,true,true,true,false};
    vector<Name*> names = {n1,n2,n3};
    auto * test = new Graph(3, std::complex<double>(1.0, 0.0), particles , names);
    vector<Graph*> superposition = vector<Graph*>(0);
    superposition.push_back(test);
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    merge_graphs(superposition);

    for(int i = 0; i<3; i++) {
        shift(superposition);
        cout << "shift \n" ;
        //cout << to_string(superposition) << "\n";
        cout << "Norm :" << calculate_norm(superposition) << "\n";
        interaction(superposition, hadamard);
        cout << "interaction \n";
        //cout << to_string(superposition) << "\n";
        cout << "Norm :" << calculate_norm(superposition) << " " << superposition.size() << "\n";
        merge_graphs(superposition);
        cout << "merge \n" ;
        //cout << to_string(superposition) << "\n";
        cout << "Norm :" << calculate_norm(superposition) << " " << superposition.size() << "\n";
        cout << superposition.size() << "\n";
    }
    for(auto &graph:superposition) {
        delete graph;
    }
    auto t1 = Time::now();
    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    std::cout << fs.count() << "s\n";
    return 0;
}
