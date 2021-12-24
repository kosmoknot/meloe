#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../include/templateManager.hpp"
#include "../include/funkyFunctions.hpp"
// #include "../include/cat.hpp"
#pragma once
using namespace std;

//this is used to render stray-cats and wikies

struct wikiInfo
{
    unordered_map<int,float> taskVals;
    string table;
    string path;
};



class pageManager
{
public:
    pageManager(TemplateManager *pTM);
    void RenderPages();
    void addWikiEntry(string wikiName, string entryText, float val, string date,int taskID);

private:
    unordered_map<string,wikiInfo> wikis;
    unordered_map<string,vector<string>> cats;
    // cat* cats;
    TemplateManager *_pTM;

    void indexWikies();
    void indexCats();
    
    void renderWikies();
    void renderCats();

    void renderHome();
};
