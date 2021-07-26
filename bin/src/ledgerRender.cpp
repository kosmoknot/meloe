#include "../include/ledgerRender.hpp"

LedgerRender::LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateConfig *pTConfig, PageConfig *pPConfig)
    : _pLConfig(pLConfig), _pPConfig(pPConfig), _pTConfig(pTConfig)
{
    this->Render(ledgerPath);
}

LedgerEntry::LedgerEntry(LedgerRender *pLRender) : _pLRender(pLRender)
{
}

void LedgerEntry::clear()
{
    this->_date.clear();
    this->_note.clear();
    this->_sectors.clear();
    this->_stats.clear();
}

void LedgerRender::Render(string path)
{
    ifstream ledgerfile;
    string line;
    ledgerfile.open(path);
    bool gotNewLineFlag = false;
    bool haveEntry = false;
    int cur = -1;
    char rune = ' ';
    LedgerEntry entry(this);
    while (ledgerfile)
    {
        if (haveEntry == false)
        {
            entry.clear();
        }
        if (gotNewLineFlag == false)
        {
            getline(ledgerfile, line);
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
            entry._date = line.substr(i + 3, i + 8);
            cur++;
            cout << "Date is " << entry._date << endl;
        }
        //rune says its a note
        else if (rune == 'n' && cur != -1)
        {
            entry._note = line.substr(i + 3, line.size());
            cout << "note is " << entry._note << endl;
        }
        // rune says its a sector
        else if (rune == 's' && cur != -1)
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
                getline(ledgerfile, line);
                i = line.find('#');
            }
            {
                for (auto sector : entry._sectors)
                {
                    cout << " sectorID = " << sector.first << " Hrs = " << sector.second.first << " note = " << sector.second.second << endl;
                }
            }
        }
        // rune says its stats info
        else if (rune == 't' && cur != -1)
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
                getline(ledgerfile, line);
                i = line.find("#");
            }
            for (auto stat : entry._stats)
            {
                cout << stat.first << " = " << stat.second << endl;
            }
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

bool LedgerEntry::Render()
{
}