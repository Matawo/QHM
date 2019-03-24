//
// Created by matawo on 22/03/19.
//
#include <stdlib.h>
#include "Name.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#define RIGHT true
#define LEFT false

SimpleName::SimpleName(int value, const vector<bool> &t) : value(value), t(t) {}

size_t SimpleName::hash() {
    return 0;
}

bool SimpleName::equals(Name *n) {
    auto sn = dynamic_cast<SimpleName*>(n);
    return sn != nullptr && this->value == sn->value&& this->t == sn->t;
}

string SimpleName::to_string() {
    string str = ::to_string(this->value) + ".";
    for(auto b : this->t) {
        str += ::to_string(b);
    }
    return str;
}

bool SimpleName::is_brother(Name *other) {
    return false;
}

SimpleName* SimpleName::get_son(bool right_or_left) {
    vector<bool> new_vector = vector<bool>(this->t);
    new_vector.push_back(right_or_left);
    auto son_addr = new SimpleName(this-> value, new_vector);
    return son_addr;
}

Name* SimpleName::get_left() {
    return this->get_son(LEFT);
}

Name* SimpleName::get_right() {
    return this->get_son(RIGHT);
}
ComposedName::ComposedName(Name *left, Name *right) : left(left), right(right) {}

size_t ComposedName::hash() {
    size_t h1 = this->left->hash();
    size_t h2 = this->right->hash();
    return h1 ^ (h2 << 1);
}


bool ComposedName::equals(Name *n) {
    auto cn = dynamic_cast<ComposedName*>(n);
    return cn != nullptr && this->left->equals(cn->left)&& this->right->equals(cn->right);
}

string ComposedName::to_string() {
    return "("+ this->left->to_string() + "^" + this->right->to_string() + ")";
}

Name *ComposedName::get_left() {
    return left;
}

Name *ComposedName::get_right() {
    return right;
}
