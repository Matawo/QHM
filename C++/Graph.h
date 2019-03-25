//
// Created by Nicolas Durbec on 25/03/19.
//

#ifndef QHM_GRAPH_H
#define QHM_GRAPH_H

#include <vector>
#include "Name.h"
#include <complex>

class Graph {
public:
    Graph(const vector<bool> &particles, Name **names);

    State* shift(std::complex<double> * unitary, std::complex<double> amp);

private:
    std::vector<bool> particles;
    Name** names;

};


#endif //QHM_GRAPH_H
