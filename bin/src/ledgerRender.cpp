#include <iomanip>
#include <sstream>
#include "../include/ledgerRender.hpp"
#include <set>

LedgerRender::LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateManager *pTManager)
    : _pLConfig(pLConfig), _pTManager(pTManager)
{
    // cout<<"LedgerRender::LedgerRender"<<endl;
    this->ReadandRender(ledgerPath);
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

void LedgerRender::ReadandRender(string path)
{
    // cout<<"LedgerRender::Render"<<endl;
    this->_ledger.open("../site/ledger.html");

    ifstream ledgerMDfile;
    string line;
    ledgerMDfile.open(path);
    bool isStartDate = true;
    bool gotNewLineFlag = false;
    char rune = ' ';
    LedgerEntry entry(this);
    while (ledgerMDfile)
    {
        if (gotNewLineFlag == false)
        {
            std::getline(ledgerMDfile, line);
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
            //set start date
            if (isStartDate == true)
            {
                this->_startDate = line.substr(i + 3, i + 8);
                isStartDate = false;
            }

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
                    auto index = entry._sectors.find(sectorID * 10 + taskID);
                    if (index != entry._sectors.end())
                    {
                        index->second.first += hrs;
                        index->second.second += (" " + parts[3]);
                    }
                    else
                    {
                        entry._sectors.insert(make_pair(sectorID * 10 + taskID, make_pair(hrs, parts[3])));
                    }
                }
                gotNewLineFlag = true;
                std::getline(ledgerMDfile, line);
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
                std::getline(ledgerMDfile, line);
                i = line.find("#");
            }
            // for (auto stat : entry._stats)
            // {
            //     cout << stat.first << " = " << stat.second << endl;
            // }
        }
    }
    entry.Render();
    ledgerMDfile.close();
    entry.close();
    this->RenderChartsandSummary();
    _ledger << "</div></body></html>";
    _ledger.close();
}

void LedgerEntry::Render()
{
    // cout<<"LedgerEntry::Render"<<endl;

    //print header if this is the first time render is called
    if (this->_date.empty() == true)
    {
        //when entering for first time render header and set start date
        this->_pLRender->_ledger << parseLinks("{{ledger-header}}", _pLRender->_pTManager);
    }
    else
    {
        string entry;
        entry = fillStatsSectors(parseLinks("{{ledger-entry:date=" + this->_date + ";note=" + this->_note + ";}}", _pLRender->_pTManager));
        this->_pLRender->_ledger << entry;
    }
}

void LedgerEntry::close()
{
    // this->_pLRender->_ledger << parseLinks("{{ledger-footer}}", _pLRender->_pTManager);
    this->_pLRender->_ledger << "</ol></div>";
}

string LedgerEntry::fillStatsSectors(string iText)
{
    //fill stats bar
    LedgerConfig *config = this->_pLRender->_pLConfig;
    if (this->_stats.size() > 0)
    {
        string temp = "";
        for (int j = 0; j < this->_stats.size(); j++)
        {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << this->_stats[j];
            string percentage = to_string(this->_stats[j] / config->_statsConfig[j].max * 100) + "%";
            temp += config->_statsConfig[j].name + ": " + ss.str() + config->_statsConfig[j].unit + "<svg class=\"graph\" width=\"100%\" height=\"10\" viewBox=\"0 0 100% 10\"><rect x=\"0\" y=\"0\" width=" + percentage + " height=\"10\" rx=\"5\" ry=\"5\" fill=\"" + config->_statsConfig[j].color + "\"/></svg>";

            //save for drawing charts
            if (this->_pLRender->statValues.find(j) != this->_pLRender->statValues.end())
            {
                this->_pLRender->statValues[j].insert(make_pair(this->_pLRender->dateToint(this->_date), this->_stats[j]));
            }
            else
            {
                unordered_map<int, float> map;
                map.insert(make_pair(this->_pLRender->dateToint(this->_date), this->_stats[j]));
                this->_pLRender->statValues.insert(make_pair(j, map));
            }
        }
        //do the insertion
        replace("&&stats&&", temp, iText);
    }
    //fill sector pie
    if (this->_sectors.size() > 0)
    {
        string pie = "";
        string legend = "";
        float total = this->dailyTotalHrs();
        float angle = 0;
        set<int> doneSectors;

        for (auto sector : this->_sectors)
        {
            int sectorID = sector.first / 10;
            if (doneSectors.find(sectorID) == doneSectors.end())
            {
                if (angle != 0)
                {
                    pie += " , ";
                }
                legend += "<div class='color-box' style='background-color:" + config->_sectorsConfig[sectorID].color + ";'></div>" + config->_sectorsConfig[sectorID].name + "<br>";
                pie += config->_sectorsConfig[sectorID].color + " " + to_string(angle) + "deg ";
                angle += this->sectorTotalHrs(sectorID) / total * 360;
                pie += to_string(angle) + "deg";
                doneSectors.insert(sectorID);
            }
        }
        if (angle != 0)
        {
            pie = "<div class = 'pie' style = 'background-image: conic-gradient(" + pie + ");'></div>";
            replace("&&sectors-pie&&", pie, iText);
            replace("&&sectors-legend&&", legend, iText);
        }
    }

    else
    {
        replace("&&sectors-pie&&", "", iText);
        replace("&&sectors-legend&&", "", iText);
    }
    return iText;
}

float LedgerEntry::dailyTotalHrs()
{
    float ret = 0;
    for (auto sector : this->_sectors)
    {
        ret += sector.second.first;
    }
    return ret;
}

float LedgerEntry::sectorTotalHrs(int sectorID)
{
    float ret = 0;

    for (auto sector : this->_sectors)
    {
        if (sector.first / 10 == sectorID)
            ret += sector.second.first;
    }

    return ret;
}

void LedgerRender::RenderChartsandSummary()
{
    //draw stats charts
    for (int i = 0; i < statValues.size(); i++)
    {
        string name = _pLConfig->_statsConfig[i].name;
        for(auto entry:statValues[i])
        {
            int date = entry.first;
            float val = entry.second;
            // cout<<date<<" : "<<val<<endl;
        }

    }
    
}

int LedgerRender::dateToint(string idate)
{
    int ret = 0;
    ret = stoi(idate.substr(0, 2)) - stoi(_startDate.substr(0, 2)) + 14 * (idate[2] - _startDate[2]);
    int years = stoi(idate.substr(3, 2)) - stoi(_startDate.substr(3, 2));
    ret += years * 365 + years % 4;
    if (stoi(_startDate.substr(3, 2)) % 4 == 0 && years != 0)
        ret++;
    return ret;
}
