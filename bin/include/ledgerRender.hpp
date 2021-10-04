#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <set>

#include "templateManager.hpp"
#include "ledgerConfig.hpp"
#include "funkyFunctions.hpp"
#include "../include/pageRender.hpp"

#pragma once

class LedgerRender
{
public:
    // static unordered_map<int, float> _TatalSectorVals;
    unordered_map<int, unordered_map<int, float>> statValues; // <stat-ID <date,value> >

    TemplateManager *pTManager;
    LedgerConfig *pLConfig;
    pageManager *pPM;
    ofstream _ledger;
    string _startDate;

    int dateToint(string idate);

    ~LedgerRender();
    LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateManager *pTManager, pageManager *pPManager);

private:
    void ReadandRender(string path);
    void RenderCharts();
};

class LedgerEntry
{
public:
    string _date;
    string _note;
    unordered_map<int, float> _stats;
    unordered_map<int, pair<float, string>> _sectors;
    //secotors => (id, (value,note)) where id = sector-id*10+ task-id

    LedgerEntry(LedgerRender *pLRender);

    void Render();
    void clear();
    void close();

private:
    LedgerRender *_pLRender;
    float dailyTotalHrs();
    float sectorTotalHrs(int sectorID);
    string fillStatsSectors(string iText);
    void RenderWiki(); //add entries in wiki pages
};