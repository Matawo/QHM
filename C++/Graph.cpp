#include <utility>

//
// Created by Nicolas Durbec on 25/03/19.
//

#include "Graph.h"
#include <set>
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

Graph* Graph::shift() {
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
