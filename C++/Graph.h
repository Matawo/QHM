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
#include <unordered_set>

class Graph {
public:
    Graph(unsigned int size,complex<double> amp, vector<bool> particles, Name **names);
    ~Graph();

    Graph* shift();

    unordered_set<Graph> * interaction(complex<double> * unitary);

    string to_string();

<<<<<<< HEAD
    Name** names_copy();
=======
    bool operator==(const Graph &rhs) const;

    bool operator!=(const Graph &rhs) const;

>>>>>>> 369ee1a8aa3d5ca1d9b256a8160d18886029bec4

private:
    unsigned int size;
    complex<double> amp;
    std::vector<bool> particles;
    Name** names;
};


#endif //QHM_GRAPH_H
