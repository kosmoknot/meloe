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
    unordered_map<int, float> taskVals;
    string table;
    string path;
};

// static string CurrTilePath = "../content/stray-cats";
class Tile
{
public:
    void render();
    void indexChildren();
    vector<Tile> _children; //this is empty if end tile
    string _title;
    string _path;
};

class pageManager
{
public:
    pageManager(TemplateManager *pTM);
    void RenderPages();
    void addWikiEntry(string wikiName, string entryText, float val, string date, int taskID);

private:
    unordered_map<string, wikiInfo> wikis;
    Tile homeTile;
    //unordered_map<string, vector<string>> cats;
    TemplateManager *_pTM;

    void indexWikies();
    void indexTiles();

    void renderWikies();
    void renderTiles();

    // void renderHome();
};
