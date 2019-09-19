#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include "Name.h"
#include "Name.cpp"
#include "Graph.h"
#include <unordered_map>
#include <mutex>
#include <omp.h>

double error_margin = pow(10.0,-20);

complex<double> calculate_norm(vector<Graph*> superposition) {
    complex<double> result = std::complex<double>(0.0,0.0);
    for (auto &i : superposition) {
        result += i->getAmp() * i->getAmp();
    }
    return result;
}

double parallel_norm(int nb_thread,unordered_multimap<size_t,Graph*>**superposition) {
    double sum[nb_thread];
    #pragma omp parallel shared(superposition,sum)
    {
        int me = omp_get_thread_num();
        sum[me]=0;
        for(auto pair:*superposition[me]) {
            sum[me]+=norm(pair.second->getAmp());
        }
    };
    #pragma barrier
    double result = 0.0;
    for(int i=0; i<nb_thread;i++) {
        result += sum[i];
    }
    return result;
}

int parallel_number(int nb_thread,unordered_multimap<size_t,Graph*>**superposition) {

    int result = 0;
    for(int i=0; i<nb_thread;i++) {
        result += superposition[i]->size();
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

void safe_insert(int nb_thread,unordered_multimap<size_t,Graph*>**superposition, Graph* graph,
        vector<std::mutex*> mutexes) {
    size_t hash = graph->hash();
    size_t i = hash%nb_thread;
    auto sorted = superposition[i];
    auto m = mutexes[hash % (nb_thread * 10)];
    //cout << " hash " << hash % (nb_thread * 10);
    while(not m->try_lock()) {}
    //cout << " <" << omp_get_thread_num();
    auto range = sorted->equal_range(hash);
    bool exist = false;
    Graph * target = graph;
    auto it = range.first;
    while(it != range.second) {
        if (graph->equals(it->second)) {
            it->second->setAmp(it->second->getAmp()+graph->getAmp());
            exist = true;
            //cout << graph->to_string_amp() << "     " <<it->second->to_string_amp() << "\n";
            delete graph;
            target = it->second;
            break;
        }
        it++;
    }
    if(not exist) {
        sorted->insert(std::pair<size_t,Graph*>(hash,graph));
    }
    if ( norm(target->getAmp())< error_margin) {
        sorted->erase(it);
        delete target;
    }
    //cout << omp_get_thread_num() << ">\n";
    m->unlock();
}


int open_mp() {
    //init
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
    const int nb_thread = 1;
    omp_set_num_threads(nb_thread);
    unordered_multimap<size_t, Graph*> *superposition[nb_thread];
    unordered_multimap<size_t, Graph*> *new_superposition[nb_thread];
    vector<std::mutex*> mutexes;
    for (auto &i : superposition) {
        i = new unordered_multimap<size_t, Graph*>(0);
    }
    for (auto &i : new_superposition) {
        i = new unordered_multimap<size_t, Graph*>(0);
    }
    mutexes.reserve(nb_thread * 10);
    for(int i=0; i< nb_thread*10;i++) {
        mutexes.push_back(new mutex);
    }
    safe_insert(nb_thread,superposition,graph_init,mutexes);

    //run
    for(int i = 0; i<5; i++) {
        cout << "shift \n" ;
        #pragma omp parallel shared(mutexes,new_superposition)
        {
            int me = omp_get_thread_num();
            new_superposition[me]->clear();
            for (auto pair:*superposition[me]) {
                safe_insert(nb_thread, new_superposition, pair.second->shift(), mutexes);
            }
            #pragma omp barrier
        }
        swap(superposition,new_superposition);
        cout << "Number : " << parallel_number(nb_thread,superposition) << "\n";
        cout << "Norm : " << parallel_norm(nb_thread,superposition) << "\n";
        cout << "interaction \n";
        #pragma omp parallel shared(mutexes, new_superposition)
        {
            int me = omp_get_thread_num();
            new_superposition[me]->clear();
            for(auto pair:*superposition[me]) {
                for(auto graph:pair.second->interaction(hadamard)) {
                    safe_insert(nb_thread,new_superposition,graph,mutexes);
                }
            }
            #pragma omp barrier
        }
        swap(superposition,new_superposition);
        for (auto &section : new_superposition) {
            section->erase(section->begin(), section->end());
        }
        cout << "Number : " << parallel_number(nb_thread,superposition) << "\n";
        cout << "Norm :" << parallel_norm(nb_thread,superposition) << "\n";
    };
    // Clean up
    for (auto &i : superposition) {
        for(auto a : *i) {
            delete a.second ;
        }
        delete i;
    }
    for (auto &i : new_superposition) {
        for(auto a : *i) {
            delete a.second ;
        }
        delete i;
    }
    for (auto &i : mutexes) {
        delete i;
    }
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
    for(int i = 0; i<10; i++) {
        shift(superposition);
        cout << "shift \n" ;
        //cout << to_string(superposition) << "\n";
        cout << "Norm :" << calculate_norm(superposition) << " " << superposition.size()<< "\n";
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
