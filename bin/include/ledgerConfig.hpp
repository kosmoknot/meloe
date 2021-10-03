#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#pragma once
using namespace std;

class StatConfig
{
public:
    string name;
    string unit;
    float max;
    string color;
};

struct Slice
{
    string name;
    string color;
};

class LedgerConfig
{
public:
    string _start;
    vector<StatConfig> _statsConfig;
    vector<Slice> _sectorsConfig;
    vector<Slice> _tasksConfig;
    void parse(string path);
    LedgerConfig(string pathConfig);
};
