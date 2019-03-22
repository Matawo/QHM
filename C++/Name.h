//
// Created by matawo on 22/03/19.
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
    virtual Name* left() = 0;
    virtual Name* right() = 0;
};

class SimpleName: public Name {
    int value;
public:
    SimpleName(int value, const vector<bool> &t);

    size_t hash() override;

    bool equals(Name *n) override;

    std::string to_string() override;

    bool is_brother(Name *other);

    Name *left() override;

    Name *right() override;

private:
    std::vector<bool> t;
};


class ComposedName: public Name {
public:
    ComposedName(Name *left, Name *right);

    size_t hash() override;

    bool equals(Name *n) override;

    std::string to_string() override;

private:
    Name *left;
    Name *right;
};

#endif //HM_QUANTIQUE_NAME_H