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
    Graph(unsigned int size,complex<double> amp, vector<bool> particles, vector<Name*> names);
    ~Graph();

    Graph* copy();

    Graph* shift();

    std::vector<Graph*> interaction(const complex<double> unitary[4]);

    string to_string();

    string to_string_amp();

    vector<Name*> names_copy();

    size_t hash();

    bool operator==(const Graph &rhs) const;

    bool operator!=(const Graph &rhs) const;

    bool operator<(const Graph &rhs) const;

    const complex<double> &getAmp() const;

    void setAmp(const complex<double> &amp);

    bool equals(Graph *other);


private:
    unsigned int size;
    complex<double> amp;
    std::vector<bool> particles;
    std::vector<Name*> names;
};


#endif //QHM_GRAPH_H
