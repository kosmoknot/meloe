#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../include/templateManager.hpp"
#include "../include/funkyFunctions.hpp"
#pragma once
using namespace std;

//this is used to render stray-cats and wikies
//need to create a wiki struct and save incoming entry data in it

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
    vector<string>cats;
    TemplateManager *_pTM;

    void indexWikies();
    void indexCats();
    
    void renderWikies();
    void renderCats();

    void renderHome();
};
