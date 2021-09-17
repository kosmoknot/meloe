#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "templateManager.hpp"
#include "pageConfig.hpp"
#include "ledgerConfig.hpp"
#include "funkyFunctions.hpp"
#pragma once

class LedgerRender
{
public:
    static unordered_map<pair<int, int>, float> _TatalSectorVals;
    ofstream _ledger;
    static unordered_map<int, float> _TotalStatVals;
    TemplateManager *_pTManager;
    LedgerConfig *_pLConfig;
    LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateManager *pTManager);
    ~LedgerRender();
    void Render(string path);
};

class LedgerEntry
{
public:
    string _date;
    string _note;
    unordered_map<int, float> _stats;
    unordered_map<int, pair<float, string>> _sectors;

    LedgerEntry(LedgerRender *pLRender);

    void Render();
    void clear();
    void close();
private:
    LedgerRender *_pLRender;
    int dailyTotalHrs();
    int sectorTotalHrs(int SectorID);
    void printout(std::string iText);
    string fillStatsSectors(string iText);
};