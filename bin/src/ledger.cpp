#include "../include/ledger.hpp"
#include "../include/funkyFunctions.hpp"

Ledger::Ledger(string pathConfig, string pathLedger, Config *pConfig, Template *pTemplate)
    : _pConfig(pConfig), _pTemplate(pTemplate)
{
    _pLedgerConfig = new LedgerConfig;
    _pLedgerConfig->parse(pathConfig);
    this->parse(pathLedger);
}
Ledger::~Ledger()
{
    delete _pConfig;
}

void LedgerConfig::parse(string path)
{
    ifstream configfile;
    string line;
    configfile.open(path);
    string cur;
    int newlineflag = 0;
    while (configfile)
    {
        if (newlineflag == 0)
        {
            getline(configfile, line);
        }
        else
        {
            newlineflag = 0;
        }

        // find the config name
        if (line.find("##") != string::npos)
        { //look for labels
            if (line.length() > 3)
            {
                cur = line.substr(line.find("##") + 3, line.size());
                cur = toLowerCase(cur);
            }
            else
            {
                cur = "";
            }

            //get start
            if (cur == "start")
            {
                string temp;
                newlineflag = 1;
                getline(configfile, line);
                while (line.find("##") == string::npos)
                {
                    temp += line;
                    getline(configfile, line);
                }
                // cout<<cur<<" = "<<temp<<endl;
                this->_start = temp;
            }
            //get stats info
            else if (cur == "stats")
            {
                vector<Stat> stats;
                newlineflag = 1;
                getline(configfile, line);
                while (line.find("##") == string::npos)
                {
                    vector<string> vals = split(line, " | ");
                    if (vals.empty() != 1)
                    {
                        Stat temp;
                        temp.name = vals[1];
                        temp.unit = vals[2];
                        temp.max = stoi(vals[3]);
                        temp.color = vals[4];
                        stats.push_back(temp);
                        // cout << temp.max << "," << temp.name << "," << temp.unit << "," << temp.color << endl;
                    }
                    getline(configfile, line);
                }
                this->_stats = stats;
            }
            //get sector and task info
            else if (cur == "sectors" || cur == "tasks")
            {
                vector<Slice> pie;
                newlineflag = 1;
                getline(configfile, line);
                while (line.find("##") == string::npos)
                {
                    vector<string> vals = split(line, " | ");
                    if (vals.empty() != 1)
                    {
                        Slice temp;
                        temp.name = vals[1];
                        temp.color = vals[2];
                        pie.push_back(temp);
                        // cout<<temp.name<<" "<<temp.color<<endl;
                    }
                    getline(configfile, line);
                }
                if (cur == "sectors")
                    this->_sectors = pie;
                else if (cur == "tasks")
                    this->_tasks = pie;
            }
        }
    }
}

void Ledger::parse(string path)
{

    ifstream ledgerfile;
    string line;
    ledgerfile.open(path);

    int cur = -1;
    char rune = ' ';
    while (ledgerfile)
    {
        getline(ledgerfile, line);
        size_t i;
        // find the rune
        i = line.find('#');
        if (i != string::npos)
        {
            rune = line[i + 1];
        }
        //rune says its a new entry
        if (rune == '#')
        {
            LedgerEntry temp;
            temp._date = line.substr(i + 3, i + 8);
            this->_Entries.push_back(temp);
            cur++;
        }
        //rune says its a note
        else if (rune == 'n' && cur != -1)
        {
            this->_Entries[cur]._note = line.substr(i + 4, line.size());
        }
        // rune says its a sector
        else if (rune == 's' && cur != -1)
        {
            while (i == string::npos || this->_Entries[cur]._sectors.size() == 0)
            {
                if (i != string::npos)
                {
                    line = line.substr(i + 2, line.size());
                }
                vector<string> parts = split(line, " | ");
                if (parts.size() == 4)
                {
                    int sectorID = stoi(parts[0]);
                    int taskID = stoi(parts[1]);
                    float hrs = stof(parts[2]);
                    if (this->_Entries[cur]._sectors.find(sectorID) != this->_Entries[cur]._sectors.end())
                    {
                        if (this->_Entries[cur]._sectors[sectorID]._taskHrs.find(taskID) != this->_Entries[cur]._sectors[sectorID]._taskHrs.end())
                        {
                            this->_Entries[cur]._sectors[sectorID]._taskHrs[taskID] += hrs;
                            this->_Entries[cur]._sectors[sectorID]._note += (" "+parts[3]);
                        }
                        else
                        {
                            this->_Entries[cur]._sectors[sectorID]._taskHrs.insert(make_pair(taskID, hrs));
                            this->_Entries[cur]._sectors[sectorID]._note = parts[3];
                        }
                    }
                    else
                    {
                        Sector temp;
                        temp._note=parts[3];
                        temp._taskHrs.insert(make_pair(taskID,hrs));
                        this->_Entries[cur]._sectors.insert(make_pair(sectorID,temp));
                    }
                }
            }
        }
        // rune says its stats info
        else if (rune == 't' && cur != -1)
        {
            i += 4;
        }
        else if (rune == 'a' && cur != -1)
        {
        }
        else if (rune == 'g' && cur != -1)
        {
        }
        else if (rune == 'l' && cur != -1)
        {
        }
    }
    ledgerfile.close();
}

void Ledger::render()
{
}