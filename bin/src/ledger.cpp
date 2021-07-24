#include "../include/ledger.hpp"
#include "../include/funkyFunctions.hpp"

Ledger::Ledger(string pathConfig,string pathLedger,Config* pConfig,Template* pTemplate)
    :_pConfig(pConfig),_pTemplate(pTemplate)
{
       _pLedgerConfig = new LedgerConfig;
       _pLedgerConfig->parse(pathConfig);
       this->parse(pathLedger);
}
Ledger::~Ledger(){
        delete _pConfig;
}

void LedgerConfig::parse(string path){
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
                if(cur=="sectors")
                    this->_sectors = pie;
                else if(cur=="tasks")
                    this->_tasks = pie;
            }
        }
    }
}

void Ledger::parse(string path){

}

void Ledger::render(){
    
}
