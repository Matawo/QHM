//
// Created by Nicolas Durbec on 22/03/19.
//
#include <vector>
#include <string>
#ifndef HM_QUANTIQUE_NAME_H
#define HM_QUANTIQUE_NAME_H

using namespace std;

class Name {
public :
    virtual size_t hash() = 0;
    virtual bool equals(Name* n) = 0;
    virtual std::string to_string() = 0;
    virtual Name* get_left() = 0;
    virtual Name* get_right() = 0;
    virtual Name* normalize() = 0;
};

class SimpleName: public Name {
public:
    std::vector<bool> t;

    int value;

    SimpleName(int value, const vector<bool> &t);

    size_t hash() override;

    bool equals(Name *n) override;

    std::string to_string() override;

    bool is_brother(Name *other);

    Name *get_left() override;

    Name *get_right() override;

    Name *normalize() override;


private:
    //True = right, false = left
    SimpleName*  get_son(bool);
};


class ComposedName: public Name {
public:
    ComposedName(Name *left, Name *right);

    size_t hash() override;

    bool equals(Name *n) override;

    std::string to_string() override;

    Name *get_left() override;

    Name *get_right() override;

    Name *normalize() override;

private:
    Name *left;
    Name *right;
};

#endif //HM_QUANTIQUE_NAME_H