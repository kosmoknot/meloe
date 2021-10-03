#include "../include/cat.hpp"

cat::cat(string iname)
    : _name(iname)
{
}

vector<cat *> cat::kittens()
{
    return _kittens;
}

string cat::name()
{
    return _name;
}

void cat::insertKitten(string name)
{
    cat *kitten = new cat(name);
    this->_kittens.push_back(kitten);
}