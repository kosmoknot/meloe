#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "template.hpp"
#pragma once
using namespace std;

class StatConfig
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

class Sector{
    public:
    string _note;
    unordered_map<int,float> _taskHrs;
};

class LedgerConfig{
    public:
    string _start;
    vector<StatConfig> _statsConfig;
    vector<Slice> _sectorsConfig; 
    vector<Slice> _tasksConfig;
    void parse(string path);
};


class LedgerEntry{
    public:
    string _date;
    string _note;
    unordered_map<int, float> _stats;
    unordered_map<int,Sector> _sectors;
    int dailyTotalHrs();
    int sectorTotalHrs(int SectorID);
};

class Ledger{
    public:
    Config* _pConfig;
    Template* _pTemplate;
    LedgerConfig* _pLedgerConfig;
    vector<LedgerEntry> _Entries;
    Ledger(string pathConfig,string pathLedger,Config* pConfig,Template* pTemplate);
    ~Ledger();
    void parse(string path);
    void render();
};
