//
// Created by Nicolas Durbec on 22/03/19.
//
#include <stdlib.h>
#include "Name.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#define RIGHT true
#define LEFT false

Name* safe_new_cn(Name * left, Name * right) {
    auto left_cn = dynamic_cast<SimpleName*>(left);
    if (left_cn == nullptr || not left_cn->is_brother(right)) {return new ComposedName(left,right);}
    left_cn->t.pop_back();
    delete right;
    return left_cn;
}

SimpleName::SimpleName(int value,const vector<bool> &t) : value(value), t(t) {}

size_t SimpleName::hash() {
    auto h1 = (size_t) value;
    auto h2 = std::hash<vector<bool>>{}(t);
    return h1 ^ (h2 << 1);
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
    auto other_ptr = dynamic_cast<SimpleName*>(other);
    if ((other_ptr == nullptr)
    or (this->value!= other_ptr->value)
    or (this->t.empty())
    or (this->t.size() != other_ptr->t.size())) {
        return false;
    }
    else {
        for(int i = 0; i < min(this->t.size() -1,other_ptr->t.size() -1) ; i++) {
            if (this->t[i] != other_ptr->t[i]) { return false;}
        }
    }
    return true;
}

SimpleName* SimpleName::get_son(bool right_or_left) {
    vector<bool> new_vector = this->t;
    new_vector.push_back(right_or_left);
    auto son_addr = new SimpleName(this-> value, new_vector);
    return son_addr;
}

Name* SimpleName::get_left_copy() {
    return this->get_son(LEFT);
}

Name* SimpleName::get_right_copy() {
    return this->get_son(RIGHT);
}

Name* SimpleName::normalize() {
    return this;
}

Name* SimpleName::deep_copy() {
    auto * s = new SimpleName(value, t);
    return s;
}

bool SimpleName::contain_anchor(int i) {
    if (value != i) {
        return false;
    }
    for (auto &&it : t) {
        if(it) {
            return false;
        }
    }
    return true;
}

SimpleName::~SimpleName() = default;

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

Name *ComposedName::get_left_copy() {
    return left->deep_copy();
}

Name *ComposedName::get_right_copy() {
    return right->deep_copy();
}

Name *ComposedName::normalize() {
    auto left_cn = dynamic_cast<SimpleName*>(left);
    if (left_cn == nullptr || !left_cn->is_brother(this->right)) { return this;}
    vector<bool> new_vector = vector<bool>(left_cn->t);
    new_vector.pop_back();
    auto new_addr = new SimpleName(left_cn->value, new_vector);
    return new_addr;
}

ComposedName::~ComposedName() {
    if (left)
        delete left;
    if (right)
        delete right;
}

Name *ComposedName::deep_copy() {
    return new ComposedName(left->deep_copy(), right->deep_copy());
}

bool ComposedName::contain_anchor(int i) {
    return left->contain_anchor(i) or right->contain_anchor(i);
}

bool Name::operator==(Name* other){
    return this->equals(other);

}bool Name::operator!=(Name* other){
    return not this->equals(other);
}

bool Name::operator<(Name *rhs) {
    cout << to_string() <<" vs"<< rhs->to_string() << " \n";
    auto left = dynamic_cast<SimpleName*>(this);
    auto right = dynamic_cast<SimpleName*>(rhs);
    if(left != nullptr and right != nullptr) {
        if (left->value != right->value) {
            return left->value < right->value;
        }
        return left->t < right->t;
    } else if (left == nullptr and right != nullptr) {
        return false;
    } else if (left != nullptr) {
        return true;
    }
    auto leftc = dynamic_cast<ComposedName*>(this);
    auto rightc = dynamic_cast<ComposedName*>(rhs);
    bool compare_left = &leftc->left < &rightc->left;
    if (compare_left) {
        return true;
    }
    else if (&leftc != &rightc) {
        return false;
    } else {
        return &leftc->right < &rightc->right;
    }


}
