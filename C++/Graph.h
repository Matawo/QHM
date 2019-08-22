//
// Created by Nicolas Durbec on 25/03/19.
//

#ifndef QHM_GRAPH_H
#define QHM_GRAPH_H

#include <vector>
#include "Name.h"
#include <complex>
#include <set>
#include <string>

class Graph {
public:
    Graph(unsigned int size,complex<double> amp, vector<bool> particles, Name **names);
    ~Graph();

    Graph* shift();

    string to_string();

    Name** names_copy();

private:
    std::vector<bool> particles;
    Name** names;
    complex<double> amp;
    unsigned int size;
};


#endif //QHM_GRAPH_H
