#include <vector>
#include <string>
#include <unordered_map>
#include "../include/cat.hpp"
#include "../include/templateManager.hpp"
#pragma once
using namespace std;

//this is used to render stray-cats and wikies

class pageManager
{
public:
    pageManager(TemplateManager *pTM);

    void RenderPages();
    void addWikiEntry(string wikiName, string entryText, float val, string date);

private:
    cat *rootCat;
    unordered_map<string, vector<string>> wikis;
    TemplateManager *_pTM;

    void indexWikies();
    void indexCats();
    void renderWikies();
    void renderCats();
    // void renderHome();
};
