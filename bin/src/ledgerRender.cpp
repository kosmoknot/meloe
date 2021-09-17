#include <iomanip>
#include <sstream>
#include "../include/ledgerRender.hpp"

LedgerRender::LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateManager* pTManager)
    : _pLConfig(pLConfig), _pTManager(pTManager)
{
    // cout<<"LedgerRender::LedgerRender"<<endl;
    this->Render(ledgerPath);
    ofstream _ledger;
}

LedgerEntry::LedgerEntry(LedgerRender *pLRender) : _pLRender(pLRender)
{
    
    // cout<<"LedgerEntry::LedgerEntry"<<endl;
}

void LedgerEntry::clear()
{
    // cout<<"LedgerEntry::Clear"<<endl;
    this->_date.clear();
    this->_note.clear();
    this->_sectors.clear();
    this->_stats.clear();
}

void LedgerRender::Render(string path)
{   
    // cout<<"LedgerRender::Render"<<endl;
    this->_ledger.open("../site/ledger.html");
    
    ifstream ledgerfile;
    string line;
    ledgerfile.open(path);

    bool gotNewLineFlag = false;
    char rune = ' ';
    LedgerEntry entry(this);
    while (ledgerfile)
    {
        if (gotNewLineFlag == false)
        {
            std::getline(ledgerfile, line);
        }
        else
        {
            gotNewLineFlag = false;
        }
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
            entry.Render();
            entry.clear();
            entry._date = line.substr(i + 3, i + 8);
            // cout << "Date is " << entry._date << endl;
        }
        //rune says its a note
        else if (rune == 'n')
        {
            entry._note = line.substr(i + 3, line.size());
            // cout << "note :" << entry._note << endl;
        }
        // rune says its a sector
        else if (rune == 's')
        {
            while (i == string::npos || entry._sectors.size() == 0)
            {
                if (i != string::npos)
                {
                    line = line.substr(i + 2, line.size());
                }
                vector<string> parts = split(line, " | ");
                if (parts.size() == 4)
                {
                    int sectorID = toInt(parts[0]);
                    int taskID = toInt(parts[1]);
                    float hrs = toFloat(parts[2]);
                    // cout << "sectorID = " << sectorID << " taskID = " << taskID << " hrs = " << hrs << endl;
                    auto index = entry._sectors.find(sectorID * 100 + taskID);
                    if (index != entry._sectors.end())
                    {
                        index->second.first += hrs;
                        index->second.second += (" " + parts[3]);
                    }
                    else
                    {
                        entry._sectors.insert(make_pair(sectorID * 100 + taskID, make_pair(hrs, parts[3])));
                    }
                }
                gotNewLineFlag = true;
                std::getline(ledgerfile, line);
                i = line.find('#');
                // cout<<line<<endl;
            }
            {
                // for (auto sector : entry._sectors)
                // {
                //     cout << " sectorID = " << sector.first << " Hrs = " << sector.second.first << " note = " << sector.second.second << endl;
                // }
            }
        }
        // rune says its stats info
        else if (rune == 't')
        {
            while (i == string::npos || entry._stats.size() == 0)
            {
                if (i != string::npos)
                {
                    line = line.substr(i + 2, line.size());
                }
                vector<string> parts = split(line, " | ");
                if (parts.size() == 2)
                {
                    int statID = stoi(parts[0]);
                    float statVal = stof(parts[1]);
                    entry._stats.insert(make_pair(statID, statVal));
                }
                gotNewLineFlag = true;
                std::getline(ledgerfile, line);
                i = line.find("#");
            }
            for (auto stat : entry._stats)
            {
                // cout << stat.first << " = " << stat.second << endl;
            }
        }
    }
    entry.Render();
    ledgerfile.close();
    entry.close();
}

void LedgerEntry::Render()
{   
    // cout<<"LedgerEntry::Render"<<endl;

    //print header if this is the first time render is called
    if (this->_date.empty() == true)
    {
        printout("{{ledger-header}}");
    }
    else
    {
        string entry;
        entry = "{{ledger-entry:date="+this->_date+";note="+this->_note+";}}";
        printout(entry);
    }
}

void LedgerEntry::printout(std::string iText)
{
    this->_pLRender->_ledger<<fillStatsSectors(parseLinks(iText,_pLRender->_pTManager));
}

void LedgerEntry::close()
{
    this->printout("{{ledger-footer}}");
    this->_pLRender->_ledger.close();
}

string LedgerEntry::fillStatsSectors(string iText)
{
    LedgerConfig* config= this->_pLRender->_pLConfig;
    string output = "";
    if (this->_stats.size() > 0)
    {
        string temp;
        for (int j = 0; j < this->_stats.size(); j++)
        {
            temp += "{stat-name}: {stat-value} {stat-unit}<svg class=\"graph\" width=\"100%\" height=\"10\" viewBox=\"0 0 100% 10\"><rect x=\"0\" y=\"0\" width={stat-percent} height=\"10\" rx=\"5\" ry=\"5\" fill=\"{stat-color}\"/></svg>";
            replace("{stat-name}", config->_statsConfig[j].name ,temp);
            replace("{stat-unit}", config->_statsConfig[j].unit, temp);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << this->_stats[j];
            replace("{stat-value}", ss.str(), temp);
            string percentage = to_string(this->_stats[j] / config->_statsConfig[j].max * 100) + "%";
            replace("{stat-percent}", percentage, temp);
            replace("{stat-color}", config->_statsConfig[j].color,temp);
        }
        replace("{stats}", temp, output);
        temp.clear();
    }
    if (this->_sectors.size() > 0)
    {
        string pie;
        string legend;
        int total = this->dailyTotalHrs();
        bool hasSector = 0;
        for (int j = 0; j < this->_sectors.size(); j++)
        {

            if (this->sectorTotalHrs(j) != 0)
            {
                if (hasSector != 0)
                {
                    pie += " , ";
                }
                hasSector = 1;
                legend += "<div class='color-box' style='background-color:" + config->_sectorsConfig[j].color + ";'></div>" + config->_sectorsConfig[j].name + "<br>";
                pie += config->_sectorsConfig[j].color + " 0 ";
                pie += to_string(float(this->sectorTotalHrs(j)) / float(total) * 360) + "deg";
            }
        }
        if (hasSector == 1)
        {
            pie = "<div class = 'pie' style = 'background-image: conic-gradient(" + pie + ");'></div>";
            replace("{sectors-pie}", pie, output);
            // cout<<pie<<endl;
            // cout<<temp<<endl;
            replace("{sectors-legend}", legend, output);
        }
    }

    else
    {

        replace("{sectors-pie}", "", output);
        replace("{sectors-legend}", "", output);
    }
    return output;
}