#include "../include/ledgerRender.hpp"

LedgerRender::LedgerRender(string ledgerPath, LedgerConfig *pLConfig, TemplateManager *pTManager, pageManager *pPManager)
    : pLConfig(pLConfig), pTManager(pTManager), pPM(pPManager)
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
                for (auto sector : entry._sectors)
                {
                    // cout << " sectorID = " << sector.first << " Hrs = " << sector.second.first << " note = " << sector.second.second << endl;
                }
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
            for (auto stat : entry._stats)
            {
                // cout << stat.first << " = " << stat.second << endl;
            }
        }
    }
    entry.Render();
    ledgerMDfile.close();
    entry.close();
    this->RenderCharts();
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
        this->_pLRender->_ledger << parseLinks("{{ledger-header}}", _pLRender->pTManager);
    }
    else
    {
        string entry;
        entry = fillStatsSectors(parseLinks("{{ledger-entry:date=" + this->_date + ";note=" + this->_note + ";}}", _pLRender->pTManager));
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
    //process stats
    //fill stats bar
    LedgerConfig *config = this->_pLRender->pLConfig;
    if (this->_stats.size() > 0)
    {
        string temp = "";
        for (int j = 0; j < this->_stats.size(); j++)
        {
            string percentage = to_string(this->_stats[j] / config->_statsConfig[j].max * 100) + "%";
            temp += config->_statsConfig[j].name + ": " + floatToString(this->_stats[j], 2) + " "+ config->_statsConfig[j].unit + "<svg class=\"graph\" width=\"100%\" height=\"10\" viewBox=\"0 0 100% 10\"><rect x=\"0\" y=\"0\" width=" + percentage + " height=\"10\" rx=\"5\" ry=\"5\" fill=\"" + config->_statsConfig[j].color + "\"/></svg>";

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
    //process sectors
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
            //draw wiki pages
            this->_pLRender->pPM->addWikiEntry(config->_sectorsConfig[sectorID].name, sector.second.second, sector.second.first, this->_date, sector.first % 10);
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

void LedgerRender::RenderCharts()
{
    //insert div and svg header
    this->_ledger << "<div>";

    //draw stats charts
    string prev_x;
    string prev_y;
    for (int i = 0; i < statValues.size(); i++)
    {
        string name = pLConfig->_statsConfig[i].name;

        //print name of the stat
        this->_ledger << "<div class=\"charts\"><h2>" + name + "</h2><div class=\"info-container\">";
        this->_ledger << "<svg class=\"stat-chart\" aria-labelledby=\"title\" role=\"img\"viewBox=\"200 0 400 400\"><g class=\"grid x-grid\" id=\"xGrid\"><line x1=\"0%\" x2=\"0%\" y1=\"1.25%\" y2=\"92.5%\"></line></g><g class=\"grid y-grid\" id=\"yGrid\"><line x1=\"0%\" x2=\"200%\" y1=\"92.50%\" y2=\"92.50%\"></line></g><g class=\"labels x-labels\">";

        string x_axis_markings = "";
        string y_axis_markings = "<g class=\"labels y-labels\">";
        string data = "<g class=\"data\" data-setname=\"" + name + "\">";
        string data_lines = "<g class=\"dataLines\">";
        string summary = "";

        float max_val = findMax(statValues[i]);

        //n = number of vals to  be rendered. max is 90
        int n = statValues[i].size();
        if (n > 89)
            n = 89;

        //render n lastest values
        for (int j = statValues[i].size() - n; j < n; j++)
        {
            float val = statValues[i][j];
            string x = to_string(j * 198.75 / (n - 1)) + "%";
            string y = to_string((1 - val / max_val) * 92.5 / 1.4 + 26.43) + "%";

            //render day numbers on x axis
            x_axis_markings += "<text x= \"" + x + "\" y=\"96%\">" + to_string(j) + "</text>";
            data += "<circle cx= \"" + x + "\" cy=\"" + y + "\" data-value=\"" + to_string(val) + "\" r= \"1%\"></circle>";
            if (j != 0)
            {
                data_lines += "<line x1=\"" + prev_x + "\" y1=\"" + prev_y + "\"x2=\"" + x + "\"" + "y2=\"" + y + "\"/>";
                prev_x = x;
                prev_y = y;
            }
            if (j == 0)
            {
                prev_x = x;
                prev_y = y;
            }
        }

        //put confidence region line

        data_lines += "<line stroke-dasharray=\"5,5\" stroke-width=\"1\" stroke=\"#A16AE8\" x1=\"0%\" y1=\"" + to_string((1 - findUCL(this->statValues[i]) / max_val / 1.4) * 92.5) + "%\"x2=\"200%\"" + "y2=\"" + to_string((1 - findUCL(this->statValues[i]) / max_val / 1.4) * 92.5) + "%\"/>";
        data_lines += "<line stroke-dasharray=\"5,5\" stroke-width=\"1\" stroke=\"#FD49A0\" x1=\"0%\" y1=\"" + to_string((1 - findLCL(this->statValues[i]) / max_val / 1.4) * 92.5) + "%\"x2=\"200%\"" + "y2=\"" + to_string((1 - findLCL(this->statValues[i]) / max_val / 1.4) * 92.5) + "%\"/>";
        data_lines += "<line stroke-dasharray=\"5,5\" stroke-width=\"1\" stroke=\"yellow\" x1=\"0%\" y1=\"" + to_string((1 - findAverage(this->statValues[i]) / max_val / 1.4) * 92.5) + "%\"x2=\"200%\"" + "y2=\"" + to_string((1 - findAverage(this->statValues[i]) / max_val / 1.4) * 92.5) + "%\"/>";

        //create 10 markings on y-axis including 0 and one above max val
        for (int j = 0; j < 15; j++)
        {
            y_axis_markings += "<text x=\"-1%\" y= \"" + to_string((14 - j) * 92.5 / 14) + "%\">" + floatToString((max_val * 1.4 / 14 * j), 1) + "</text>";
        }

        x_axis_markings += "<text x=\"100%\" y=\"99%\" class=\"label-title\">days</text></g>";
        y_axis_markings += "<text x=\"-10%\" y=\"50%\" class=\"label-title\">" + this->pLConfig->_statsConfig[i].unit + "</text></g>";

        //calculate summary
        summary += "<li>Average: <b style=\"color:yellow;\">" + floatToString(findAverage(this->statValues[i]), 2) + "</b></li>";
        summary += "<li>Control limits: <b style=\"color:#A16AE8;\">" + floatToString(findUCL(this->statValues[i]), 2) + "</b> , <b style=\"color:#FD49A0;\">" + floatToString(findLCL(this->statValues[i]), 2) + "</b></li>";
        summary += "<li>σ: <b>" + floatToString(findSD(this->statValues[i]), 2) + "</b></li>";
        summary += "<li>30 DMA: <b>" + floatToString(findDMA(this->statValues[i], 30), 2) + "</b></li>";
        summary += "<li>90 DMA: <b>" + floatToString(findDMA(this->statValues[i], 90), 2) + "</b></li>";
        summary += "<li>300 DMA: <b>" + floatToString(findDMA(this->statValues[i], 300), 2) + "</b></li>";

        //insert calculated strings
        this->_ledger
            << x_axis_markings;
        this->_ledger << y_axis_markings;
        this->_ledger << data << "</g>";
        this->_ledger << data_lines << "</g></svg><ul class=\"stats\">";

        //insert summary
        this->_ledger << summary;
        this->_ledger << "</ul></div></div><br> ";
    }
    this->_ledger << "</div>";
}
