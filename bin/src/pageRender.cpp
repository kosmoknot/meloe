#include "../include/pageRender.hpp"

namespace fs = std::filesystem;

pageManager::pageManager(TemplateManager *pTM)
    : _pTM(pTM)
{
    indexWikies();
    indexCats();
}

void pageManager::indexWikies()
{
    std::string path = "../content/wikies";
    for (const auto &entry : fs::directory_iterator(path))
    {
        wikiInfo temp;
        temp.path = entry.path();
        // cout << entry.path() << endl;
        vector<string> tokens = tokenizer(entry.path(), "/.");
        cout << tokens[5] << endl;
        this->wikis.insert(make_pair(tokens[5], temp));
    }
}
void pageManager::renderWikies()
{
    for (auto wiki : this->wikis)
    {

        ifstream wikiMDFile;
        ofstream wikiHTMLFile;
        wikiMDFile.open(wiki.second.path);
        wikiHTMLFile.open("../site/" + wiki.first + ".html");
        string iline;
        while (wikiMDFile)
        {
            getline(wikiMDFile, iline);

            if (iline.find("&&wiki-entries&&") != string::npos)
            {
                wikiHTMLFile << wiki.second.table;
            }
            else
            {
                wikiHTMLFile << parseLinks(iline, this->_pTM);
            }
        }
        wikiHTMLFile.close();
        wikiMDFile.close();
    }
}

void pageManager::addWikiEntry(string wikiName, string entryText, float val, string date, int taskID)
{
    if (this->wikis.find(wikiName) != wikis.end())
    {
        wikis[wikiName].table += parseLinks("{{wiki-entry:date=" + date + " ;text=" + entryText + " ;value=" + floatToString(val, 2) + " ;}}", this->_pTM);
        if (wikis[wikiName].taskVals.find(taskID) == wikis[wikiName].taskVals.end())
        {
            wikis[wikiName].taskVals.insert(make_pair(taskID, val));
        }
        else
        {
            wikis[wikiName].taskVals[taskID] += val;
        }
    }
}

void pageManager::RenderPages()
{
    renderWikies();
    renderCats();
}

void pageManager::indexCats()
{
    std::string path = "../content/stray-cats";
    for (const auto &entry : fs::recursive_directory_iterator(path))
    {
        string path = entry.path();
        if (path.find(".md") != string::npos)
        {
            this->cats.push_back(entry.path());
            // cout<<entry.path()<<endl;
        }
    }
}

void pageManager::renderCats()
{
    for (string path : this->cats)
    {
        vector<string> temp = tokenizer(path, "/.");
        string name = temp[temp.size() - 2];
        // cout<<name<<endl;

        ifstream catMDFile;
        ofstream catHTMLFile;
        catMDFile.open(path);
        catHTMLFile.open("../site/" + name + ".html");
        string iline;
        while (catMDFile)
        {
            getline(catMDFile, iline);
            catHTMLFile << parseLinks(iline, this->_pTM);
        }
        catHTMLFile.close();
        catMDFile.close();
    }
}

void pageManager::renderHome()
{
}