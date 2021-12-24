#include "../include/pageRender.hpp"

namespace fs = std::filesystem;

pageManager::pageManager(TemplateManager *pTM)
    : _pTM(pTM)
{
    cats = new cat;
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
        // cout << tokens[5] << endl;
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
    renderHome();
}

void pageManager::indexCats()
{
    std::string path = "../content/stray-cats";
    for (const auto &entry : fs::recursive_directory_iterator(path))
    {
        string read_path = entry.path();
        if (read_path.find(".md") != string::npos)
        {
            vector<string> read = tokenizer(read_path, "/.");
            // cout<<read[5]<<endl;
            if (this->cats.find(read[5]) != this->cats.end())
            {
                cats[read[5]].push_back(read_path);
            }
            else
            {
                vector<string> v;
                this->cats.insert(make_pair(read[5], v));
            }
        }
    }
}

void pageManager::renderCats()
{
    for (auto cat : this->cats)
    {
        for (auto path : cat.second)
        {
            vector<string> temp = tokenizer(path, "/.");
            string name = temp[temp.size() - 2];
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

        //render pages for home tiles
        string name = cat.first;
        ofstream catHTMLFile;
        catHTMLFile.open("../site/" + name + ".html");
        catHTMLFile<<parseLinks("{{tile-header: name="+name+"}}",this->_pTM);
        unordered_map<string, vector<string>> organisedCats = organiseStrayCats(cat.second);

        

        catHTMLFile.close();
    }
}

void pageManager::renderHome()
{
    ofstream homeHTML;
    homeHTML.open("../site/home.html");
    homeHTML << parseLinks("{{home-header}}", this->_pTM);

    homeHTML << parseLinks("{{home-entry:name=Activity wikies;path=./wikies.html;}}", this->_pTM);

    for (auto cat : this->cats)
    {
        string path = "./" + cat.first + ".html";
        homeHTML << parseLinks("{{home-entry:name=" + cat.first + ";path=" + path + ";}}", this->_pTM);
    }

    homeHTML << parseLinks("{{home-footer}}", this->_pTM);
    homeHTML.close();
}
