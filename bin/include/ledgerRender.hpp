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
    static unordered_map<int, float> _TotalStatVals;
    TemplateManager *_pTManager;
    PageConfig *_pPConfig;
    LedgerConfig *_pLConfig;
    LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateManager *pTManager, PageConfig *pPConfig);
    ~LedgerRender();
    void Render(string path);
    void RenderStart();
    void RenderEnd();
};

class LedgerPage
{
public:
    string path;
    string title;
    int type;
    char rune;
};

class LedgerEntry
{
public:
    LedgerRender *_pLRender;
    string _date;
    string _note;
    vector<LedgerPage> _pages;
    unordered_map<int, float> _stats;
    unordered_map<int, pair<float, string>> _sectors;
    int dailyTotalHrs();
    int sectorTotalHrs(int SectorID);
    bool Render();
    void clear();
    LedgerEntry(LedgerRender *pLRender);
};