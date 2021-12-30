#include "../include/pageRender.hpp"

namespace fs = std::filesystem;

pageManager::pageManager(TemplateManager *pTM)
    : _pTM(pTM)
{
    // cout<<"pageManager::pageManager"<<endl;
    _homeTile = new Tile;
    _homeTile->_title = "home";
    _homeTile->_pPM = this;

    // indexWikies();
    indexTiles();
}

pageManager::~pageManager()
{
    delete _homeTile;
}

void pageManager::RenderPages()
{
    cout << "pageManager::RenderPages" << endl;
    // renderWikies();
    renderTiles();
}

void pageManager::indexTiles()
{
    // cout << "pageManager::indexTiles" << endl;
    int ChildCount = GetChildrenCount("../content/stray-cats");
    vector<Tile *> Children(ChildCount);
    for (const auto &entry : fs::directory_iterator("../content/stray-cats"))
    {
        string title;
        int ChildIndex;
        Tile *Child = new Tile;
        bool hasChildren = false;

        string read_path = entry.path();
        vector<string> read = tokenizer(read_path, "/.");

        // cout << read_path << endl;

        if (read[read.size() - 1] != "md")
        {
            title = read[read.size() - 1];
            hasChildren = true;
        }
        else
            title = read[read.size() - 2];

        // cout << title << endl;

        vector<string> TitleTokens = tokenizer(title, "_");

        if (2 == TitleTokens.size())
        {
            ChildIndex = toInt(TitleTokens[0]);
            Child->_title = TitleTokens[1];
        }

        // cout << Child->_title << endl;

        Child->_path = read_path;
        Child->_pPM = this;

        if (ChildIndex <= ChildCount)
            Children[ChildIndex - 1] = Child;

        if (true == hasChildren)
            Child->indexChildren();
    }
    _homeTile->_children = Children;
}

void Tile::indexChildren()
{
    int ChildCount = GetChildrenCount(_path);
    vector<Tile *> Children(ChildCount);
    for (const auto &entry : fs::directory_iterator(_path))
    {
        string title;
        int ChildIndex;
        Tile *Child = new Tile;
        bool hasChildren = false;

        string read_path = entry.path();
        vector<string> read = tokenizer(read_path, "/.");

        // cout << read_path << endl;

        if (read[read.size() - 1] != "md")
        {
            title = read[read.size() - 1];
            hasChildren = true;
        }
        else
            title = read[read.size() - 2];

        vector<string> TitleTokens = tokenizer(title, "_");

        if (2 == TitleTokens.size())
        {
            ChildIndex = toInt(TitleTokens[0]);
            Child->_title = TitleTokens[1];
        }

        Child->_path = read_path;
        Child->_pPM = this->_pPM;

        // cout << "child: " + Child->_title + " index: " << ChildIndex << " ChildCount: " << ChildCount << endl;
        if (ChildIndex <= ChildCount)
        {
            Children[ChildIndex - 1] = Child;
            // cout << "Added child: " + Child._title << endl;
        }

        if (true == hasChildren)
            Child->indexChildren();
    }
    _children = Children;
    cout << _title + " child-count: " << _children.size() << endl;
}

void pageManager::renderTiles()
{
    _homeTile->render();
}

void Tile::render()
{
    cout << "render: " << _title << " has children: " << _children.size() << endl;
    if (_children.size() == 0)
    {
        // leaf node --> render the page
        _pPM->renderPage(this);
    }
    else
    {
        // Not a leaf node --> render index
        _pPM->renderIndex(this);
        for (auto child : this->_children)
            child->render();
    }
}

void pageManager::renderIndex(Tile *iTile)
{
    ofstream pageHTMLFile;

    cout << "renderIndex: " << iTile->_title << endl;

    if ("home" == iTile->_title)
    {
        pageHTMLFile.open("../site/" + iTile->_title + ".html");
        pageHTMLFile << parseLinks("{{home-header}}", this->_pTM);
    }
    else
    {
        pageHTMLFile.open("../site/" + iTile->_title + "-index" + ".html");
        pageHTMLFile << parseLinks("{{index-header: name=" + iTile->_title + "}}", this->_pTM);
    }

    for (auto child : iTile->_children)
    {
        pageHTMLFile << parseLinks("{{tile: name=" + child->_title + "}}", this->_pTM);
    }

    if ("Home" == iTile->_title)
        pageHTMLFile << parseLinks("{{home-footer}}", this->_pTM);
    else
        pageHTMLFile << parseLinks("{{index-footer}}", this->_pTM);
}

void pageManager::renderPage(Tile *iTile)
{
    cout << "renderPage: " << iTile->_title << endl;
    ifstream pageMDFile;
    ofstream pageHTMLFile;
    pageMDFile.open(iTile->_path);
    pageHTMLFile.open("../site/" + iTile->_title + ".html");
    string iline;
    while (pageMDFile)
    {
        getline(pageMDFile, iline);
        pageHTMLFile << parseLinks(iline, this->_pTM);
    }
    pageHTMLFile.close();
    pageMDFile.close();
}

Tile::~Tile()
{
    for (auto child : _children)
    {
        if (NULL != child)
            delete child;
    }
}

////////////////////////////////
////////////////////////////////
////////////////////////////////
/// Wiki rendering --- TODO ----
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
// void pageManager::indexCats()
// {
//     std::string path = "../content/stray-cats";
//     for (const auto &entry : fs::recursive_directory_iterator(path))
//     {
//         string read_path = entry.path();
//         if (read_path.find(".md") != string::npos)
//         {
//             vector<string> read = tokenizer(read_path, "/.");
//             // cout<<read[5]<<endl;
//             if (this->cats.find(read[5]) != this->cats.end())
//             {
//                 cats[read[5]].push_back(read_path);
//             }
//             else
//             {
//                 vector<string> v;
//                 this->cats.insert(make_pair(read[5], v));
//             }
//         }
//     }
// }

// void pageManager::renderCats()
// {
// for (auto cat : this->cats)
// {
//     for (auto path : cat.second)
//     {
//         vector<string> temp = tokenizer(path, "/.");
//         string name = temp[temp.size() - 2];
//         ifstream catMDFile;
//         ofstream catHTMLFile;
//         catMDFile.open(path);
//         catHTMLFile.open("../site/" + name + ".html");
//         string iline;
//         while (catMDFile)
//         {
//             getline(catMDFile, iline);
//             catHTMLFile << parseLinks(iline, this->_pTM);
//         }
//         catHTMLFile.close();
//         catMDFile.close();
//     }

//     //render pages for home tiles
//     string name = cat.first;
//     ofstream catHTMLFile;
//     catHTMLFile.open("../site/" + name + ".html");
//     catHTMLFile << parseLinks("{{tile-header: name=" + name + "}}", this->_pTM);
//     unordered_map<string, vector<string>> organisedCats = organiseStrayCats(cat.second);

//     catHTMLFile.close();
// }
// }

// void pageManager::renderHome()
// {
// ofstream homeHTML;
// homeHTML.open("../site/home.html");
// homeHTML << parseLinks("{{home-header}}", this->_pTM);

// homeHTML << parseLinks("{{home-entry:name=Activity wikies;path=./wikies.html;}}", this->_pTM);

// for (auto cat : this->cats)
// {
//     string path = "./" + cat.first + ".html";
//     homeHTML << parseLinks("{{home-entry:name=" + cat.first + ";path=" + path + ";}}", this->_pTM);
// }

// homeHTML << parseLinks("{{home-footer}}", this->_pTM);
// homeHTML.close();
// }
