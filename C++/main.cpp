#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include "Name.h"
#include "Name.cpp"
#include "Graph.h"
#include <unordered_map>
#include <mutex>

double error_margin = pow(10.0,-20);

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

string to_string(vector<Graph*>superposition) {
    string result;
    for(auto &graph:superposition) {
        result += graph->to_string_amp() + "\n";
    }
    return result;
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
               //cout << graph->to_string_amp() << "     " <<it->second->to_string_amp() << "\n";
               delete graph;
           }
        }
        if(not exist) {
            sorted.insert(std::pair<size_t,Graph*>(hash,graph));
        }
        superposition.pop_back();
    }
    for (auto &it : sorted) {
        if ( norm(it.second->getAmp())> error_margin) {
            superposition.push_back(it.second);
        } else {
            delete it.second;
        }
    }
}

void merge_graphs2(vector<Graph*>&superposition) {
    //cout << to_string(superposition) << "\n";
    std::sort(superposition.begin(),superposition.end(), compare_graph_ptr);
    vector<Graph*> result = vector<Graph*>(0);
    result.push_back(superposition.back());
    superposition.pop_back();
    while(not superposition.empty()) {
        if(superposition.back()->equals(result.back())) {
            result.back()->setAmp(result.back()->getAmp()+superposition.back()->getAmp());
        } else {
            result.push_back(superposition.back());
        }
        superposition.pop_back();
        if(norm(result.back()->getAmp()) <= error_margin) {
            result.pop_back();
        }
    }
    superposition = std::move(result);
    std::sort(superposition.begin(),superposition.end(), compare_graph_ptr);
    //cout << to_string(superposition) << "\n";
}

void shift(vector<Graph*>&superposition) {
    vector<Graph*> result = vector<Graph*>(0);
    for(auto &graph:superposition) {
        result.push_back(graph->shift());
        delete graph;
    }
    superposition = result;
}

void parallel_shift(vector<Graph*>&superposition) {
    vector<Graph*> result = vector<Graph*>(0);
    for(auto &graph:superposition) {
        result.push_back(graph->shift());
        delete graph;
    }
    superposition = result;
}

void safe_insert(int nb_thread,unordered_multimap<size_t,Graph*>**superposition, Graph* graph, unordered_map<size_t,mutex*> &mutexes, mutex &master) {
    size_t hash = graph->hash();
    size_t i = hash%nb_thread;
    auto sorted = superposition[i];
    while (mutexes.find(hash) == mutexes.end()) {
        while (not master.try_lock()) {}
        if(mutexes.find(hash) == mutexes.end()) {
            auto * m = new mutex();
            mutexes.insert(std::pair<size_t,mutex*>(hash,m));
        }
        master.unlock();
    }
    while(not mutexes[hash]->try_lock()) {
        auto range = superposition[i]->equal_range(hash);
        bool exist = false;
        for(auto it = range.first; it!=range.second; it++) {
            if (graph->equals(it->second)) {
                it->second->setAmp(it->second->getAmp()+graph->getAmp());
                exist = true;
                //cout << graph->to_string_amp() << "     " <<it->second->to_string_amp() << "\n";
                delete graph;
            }
        }
        if(not exist) {
            sorted.insert(std::pair<size_t,Graph*>(hash,graph));
        }
    }
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

int open_mp() {
    SimpleName* n1 = new SimpleName(1,{});
    SimpleName* n2 = new SimpleName(2,{});
    SimpleName* n3 = new SimpleName(3,{});
    const std::complex<double> hadamard[4] = {std::complex<double>(1/sqrt(2),0.0),std::complex<double>(1/sqrt(2),0.0),
                                              std::complex<double>(1/sqrt(2),0.0),std::complex<double>(-1/sqrt(2),0.0)};
    vector<bool> particles = {false,true,true,true,true,false};
    vector<Name*> names = {n1,n2,n3};
    auto * graph_init = new Graph(3, std::complex<double>(1.0, 0.0), particles , names);
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();
    const int nb_thread = 4;

    // the table is now initialized

    return 0;
}

int sequentiel() {
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
    for(int i = 0; i<5; i++) {
        shift(superposition);
        cout << "shift \n" ;
        //cout << to_string(superposition) << "\n";
        cout << "Norm :" << calculate_norm(superposition) << "\n";
        interaction(superposition, hadamard);
        cout << "interaction \n";
        //cout << to_string(superposition) << "\n";
        cout << "Norm :" << calculate_norm(superposition) << " " << superposition.size() << "\n";
        merge_graphs(superposition);
        cout << "merge \n";
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

int main(){
    open_mp();
}
