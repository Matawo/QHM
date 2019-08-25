#include <utility>

//
// Created by Nicolas Durbec on 25/03/19.
//

#include "Graph.h"
#include <unordered_set>
#include <iostream>

Graph::Graph(unsigned int size,complex<double> amp, vector<bool> particles, Name **names) : particles(std::move(
        particles)), names(names), amp(amp), size(size) {}

string Graph::to_string() {
    string str;
    str += std::to_string(this->amp.real()) + '+' + std::to_string(this->amp.imag()) + "i ";
    for(int i=0; i<this->size;i++) {
        str += this->names[i]->to_string() + ":";
        if(this->particles[2*i]) {
            str += "l";
        }
        if(this->particles[2*i+1]) {
            str += "r";
        }
        if(!this->particles[2*i] && !this->particles[2*i+1]) {
            str += "O";
        }
        str += " ";
    }
    return str;
}

Name** Graph::names_copy() {
    Name** new_names = new Name*[sizeof(this->names)];
    for(int i = 0; i<size; i++) {
        new_names[i] = names[i]->deep_copy();
    }
    return new_names;
}


//Même pointeurs de noms,
Graph * Graph::shift() {
    vector<bool> p(2*size,false);
    for(int i=0; i<size;i++) {
        p[2*i]= this->particles[(2*i+2)%(2*size)];
        p[2*i+1]= this->particles[(2*i-1+2*size)%(2*size)];
    }
    return new Graph(size,amp, p, this->names_copy());
}

Graph::~Graph() {
    if(names) {
        for(int i=0; i< size; i++) {
            delete names[i];
        }
        delete[] names;
    }
}

unordered_set<Graph>* Graph::interaction(complex<double> * unitary) {
    auto cur_s = new unordered_set<Graph>();
    auto new_s = new unordered_set<Graph>();
    cur_s->insert(this);
    for(int i=0;i<size;i++){
        while(!cur_s->empty()) {

        }
    }
    return cur_s;
}

bool Graph::operator<(const Graph &rhs) const {
    if (size < rhs.size)
        return true;
    if (rhs.size < size)
        return false;
    if (particles < rhs.particles)
        return true;
    if (rhs.particles < particles)
        return false;
    return names < rhs.names;
}

bool Graph::operator==(const Graph &rhs) const {
    return size == rhs.size &&
           particles == rhs.particles &&
           names == rhs.names;
}

bool Graph::operator!=(const Graph &rhs) const {
    return !(rhs == *this);
}
