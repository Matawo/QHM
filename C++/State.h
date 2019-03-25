//
// Created by matawo on 25/03/19.
//

//ABORT
#ifndef QHM_STATE_H
#define QHM_STATE_H


#include "Graph.h"
#include <complex>

class State {
public:
    State(Graph **graphs, const complex<double> &amps);

    State(Graph **graphs, unsigned int size, const complex<double> &amps);

private:
    Graph** graphs;
    unsigned int size;
    std::complex<double> amps;
};


#endif //QHM_STATE_H
