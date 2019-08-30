#include <utility>

#include <utility>

//
// Created by Nicolas Durbec on 25/03/19.
//

#include "Graph.h"
#include <unordered_set>
#include <iostream>
inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    hash_combine(seed, rest...);
}

Graph::Graph(unsigned int size,complex<double> amp, vector<bool> particles, vector<Name*> names) : particles(std::move(
        particles)), names(std::move(names)), amp(amp), size(size) {}

string Graph::to_string_amp() {
    string str;
    str += std::to_string(this->amp.real()) + '+' + std::to_string(this->amp.imag()) + "i ";
    str += this->to_string();
    return str;
}

Graph* Graph::copy() {
//    cout << to_string();
    return new Graph(size, amp, particles, this->names_copy());
}

string Graph::to_string() {
    string str;
//    cout << "string of " << this << " fin string of\n";
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

vector<Name*> Graph::names_copy() {
    vector<Name*> new_names = std::vector<Name*>(size);
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
    for(int i=0; i< size; i++) {
        delete names[i];
    }
    names.clear();
}

std::vector<Graph*> Graph::interaction(const complex<double> unitary[4]) {
    auto cur_s = std::vector<Graph*>(0);
    std::vector<Graph*> new_s;
    cur_s.push_back(new Graph(0,amp,{},{}));
    int i = 0;
    while (i < size) { //On parcourt les noeuds un à un, le cas du noeud final est géré séparemment
   //     cout << "i = " << i << "\n";
        int merge = false;
        while(!cur_s.empty()) {
            auto* inert_graph = cur_s.back();
            auto* active_graph = inert_graph->copy();
//            cout << "check " << inert_graph << "check cur_s\n";
//            cout << "debut copy " << inert_graph->copy() << "fin copy\n";
            inert_graph->particles.push_back(particles[i*2]);
            inert_graph->particles.push_back(particles[i*2+1]);
            inert_graph->names.push_back(this->names[i]->deep_copy());
            inert_graph->size += 1;
            if (i and particles[i*2] and particles[i*2+1]) { // Cas de split, si i!=0
                // Cas sans split
                inert_graph->amp = inert_graph->amp * unitary[0];
                // Cas avec split
                active_graph->particles.push_back(true);
                active_graph->particles.push_back(false);
                active_graph->names.push_back(this->names[i]->get_left_copy());
                active_graph->size++;
                active_graph->particles.push_back(false);
                active_graph->particles.push_back(true);
                active_graph->names.push_back(this->names[i]->get_right_copy());
                active_graph->size++;
                active_graph->amp = active_graph->amp * unitary[1];
                new_s.push_back(active_graph);
                // Cas de merge L--R
            } else if (i<size-1 and particles[i*2] and (not particles[i*2+1])
            and (not particles[i*2+2]) and particles[i*2+3]) {
                // Cas sans merge
                inert_graph->particles.push_back(false);
                inert_graph->particles.push_back(true);
                inert_graph->size +=1;
                inert_graph->amp = inert_graph->amp * unitary[3];
                inert_graph->names.push_back(this->names[i+1]);
                // Cas avec merge
                active_graph->particles.push_back(true);
                active_graph->particles.push_back(true);
                active_graph->size += 1;
                active_graph->amp = active_graph->amp * unitary[2];
                active_graph->names.push_back(
                        new ComposedName(
                                this->names[i]->deep_copy(),
                                this->names[i+1]->deep_copy()));
                new_s.push_back(active_graph);
                merge = true;
            } else{
                delete active_graph;
            }
            new_s.push_back(inert_graph);
            cur_s.pop_back();
        }
        cur_s = std::move(new_s);
        new_s.clear();
        i+=1+merge;
    }
    while(!cur_s.empty()){ //Gestion des cas particuliers (merge begin_end et split begin)
        auto* inert_graph = cur_s.back();
        if(particles[0] and particles[1]) { //Split begining
            auto* active_graph = inert_graph->copy();
            inert_graph->amp = inert_graph->amp * unitary[0];
            active_graph->amp = active_graph->amp * unitary[1];
            Name * name = names[0];
            auto* right = name->get_right_copy();
            auto* left = name->get_left_copy();
            active_graph->particles[0] = false;
            active_graph->particles[1] = true;
            delete active_graph->names[0];
            active_graph->names[0] = right;
            active_graph->size++;
            if(left->contain_anchor(1)) {//Ajout sur place
                active_graph->particles.insert(active_graph->particles.begin(), false);
                active_graph->particles.insert(active_graph->particles.begin(), true);
                active_graph->names.insert(active_graph->names.begin(), left);
            } else {
                active_graph->particles.push_back(true);
                active_graph->particles.push_back(false);
                active_graph->names.push_back(left);
            }
            new_s.push_back(active_graph);
        } else if (particles[2*size-2] and particles[1] and not particles[0] and not particles[2*size-1]) {
            auto* active_graph = inert_graph->copy();
            Name * name = active_graph->names[0];
            inert_graph->amp = inert_graph->amp * unitary[3];
            active_graph->amp = active_graph->amp * unitary[2];

            active_graph->names[0]= new ComposedName(active_graph->names[active_graph->size-1],name);
            active_graph->names.pop_back();
            active_graph->particles[0]=true;
            active_graph->size--;
            active_graph->particles.pop_back();
            active_graph->particles.pop_back();
            new_s.push_back(active_graph);
        }
        new_s.push_back(inert_graph);
        cur_s.pop_back();
    }
    cur_s.clear();
    return new_s;
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

const complex<double> &Graph::getAmp() const {
    return amp;
}

void Graph::setAmp(const complex<double> &amp) {
    Graph::amp = amp;
}

size_t Graph::hash() {
    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(this->to_string());
    return str_hash;
}

bool Graph::equals(Graph *other) {
    return size == other->size &&
           particles == other->particles &&
           names == other->names;
}

