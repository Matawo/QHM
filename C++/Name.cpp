//
// Created by matawo on 22/03/19.
//
#include <stdlib.h>
#include "Name.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>


SimpleName::SimpleName(int value, const vector<bool> &t) : value(value), t(t) {}

size_t SimpleName::hash() {
    return 0;
}

bool SimpleName::equals(Name *n) {
    SimpleName* sn = dynamic_cast<SimpleName*>(n);
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

Name *SimpleName::left() {
    return nullptr;
}

Name *SimpleName::right() {
    return nullptr;
}

ComposedName::ComposedName(Name *left, Name *right) : left(left), right(right) {}

size_t ComposedName::hash() {
    size_t h1 = this->left->hash();
    size_t h2 = this->right->hash();
    return h1 ^ (h2 << 1);
}


bool ComposedName::equals(Name *n) {
    ComposedName* cn = dynamic_cast<ComposedName*>(n);
    return cn != nullptr && this->left->equals(cn->left)&& this->right->equals(cn->right);
}

string ComposedName::to_string() {
    return "("+ this->left->to_string() + "^" + this->right().to_string() + ")";
}
