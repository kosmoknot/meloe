#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "template.hpp"
#pragma once
using namespace std;

class Stat
{
public:
    string name;
    string unit;
    int max;
    string color;
};
struct Slice
{
    string name;
    string color;

};

class LedgerConfig{
    public:
    string _start;
    vector<Stat> _stats;
    vector<Slice> _sectors; 
    vector<Slice> _tasks;
    void parse(string path);
};

class LedgerEntry{
    public:
    string date;
    string notes;
    vector<vector<int>> sectors;
    unordered_map<int, float> stats;
    int dailyTotalHrs();
    int sectorTotalHrs(int sector_id);
};

class Ledger{
    public:
    Config* _pConfig;
    Template* _pTemplate;
    LedgerConfig* _pLedgerConfig;
    vector<LedgerEntry> Entries;
    Ledger(string pathConfig,string pathLedger,Config* pConfig,Template* pTemplate);
    ~Ledger();
    void parse(string path);
    void render();
    
};