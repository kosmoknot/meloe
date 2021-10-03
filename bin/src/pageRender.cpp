#include "../include/pageRender.hpp"
#include "../include/funkyFunctions.hpp"
#include <filesystem>
namespace fs = filesystem;


pageManager::pageManager(TemplateManager *pTM)
    : _pTM(pTM)
{
    indexWikies();
    indexCats();
}

void pageManager::indexWikies()
{
    std::string path = "/path/to/directory";
    for (const auto &entry : fs::directory_iterator(path))
}
void pageManager::indexCats()
{
}
void pageManager::renderWikies()
{
}
void pageManager::renderCats()
{
}

void pageManager::addWikiEntry(string wikiName, string entryText, float val, string date,int taskID)
{
    if (this->wikis.find(wikiName) != wikis.end())
    {
        string text = "{{wiki-entry date=" + date + " text =" + entryText + " value=" + to_string(val) + " }}";
        wikis[wikiName].push_back(parseLinks(text, this->_pTM));
    }
}

void pageManager::RenderPages()
{
}