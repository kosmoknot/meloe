#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "../include/funkyFunctions.hpp"
#include "../include/templateManager.hpp"
#include "../include/pageRender.hpp"

using namespace std;

vector<string> split(string input, string token)
{
    vector<string> output;
    int ts = token.size();
    int start = 0;
    int end = input.find(token);
    if (end == -1)
    {
        return output;
    }
    while (end != -1)
    {
        output.push_back(input.substr(start, end - start));
        start = end + ts;
        end = input.find(token, start);
    }
    output.push_back(input.substr(start, input.size() - ts + 1 - start));
    return output;
}

// orignal function replaced with same function from mizi
//  string toLowerCase(string input)
//  {
//      transform(input.begin(), input.end(), input.begin(), ::tolower);
//      return input;
//  }

string toUpperCase(string input)
{
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}

int toInt(string input)
{
    if (input.find_first_not_of("1234567890 ") == string::npos)
    {
        return stoi(input);
    }
    return -1;
}

float toFloat(string input)
{
    if (input.find_first_not_of(".1234567890 ") == string::npos)
    {
        return stof(input);
    }
    return -1;
}
char getLower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - ('Z' - 'z');
    return c;
}

// taken from mizi
string toLowerCase(string text)
{
    string newtext = "";
    for (unsigned int i = 0; i < text.size(); i++)
    {
        if (text[i] == '\0')
            break;
        else if (text[i] == ' ')
            newtext += '_';
        else if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z')) // alphabets
            newtext += getLower(text[i]);
        else if (text[i] >= '0' && text[i] <= '9')
            newtext += getLower(text[i]);
    }
    return newtext;
}
// Prints errorline
void printError(int linenumber, string text)
{
    cout << "Error at line " << linenumber << ". " << text << endl;
}

//  [urlText](url) ->  <a href='url'>urlText</a>
// ![altText](image) -> <img src='' alt=''>
// {{templateName param1=value1 param2=value2}}
string parseLinks(string text, TemplateManager *templateMgr)
{
    string label = text;
    size_t index = 0;
    string newText = "";
    string urlText = "";
    string url = "";
    bool isImage = false;

    for (; index < text.size();)
    {

        // Check if template
        if (text[index] == '{' && index + 1 < text.size() && text[index + 1] == '{')
        {
            index += 2;

            string templateString = "";
            bool templateParseSuccessful = false;
            while (index < text.size())
            {
                if (text[index] == '}' && text[index + 1] == '}')
                {
                    templateParseSuccessful = true;
                    break;
                }
                templateString += text[index];
                index += 1;
            }

            if (templateParseSuccessful)
            {
                newText += templateMgr->templateReaderParser(templateString);
                index += 2;
            }
            // if bad template
            else
            {
                newText += "{{" + templateString;
            }
        }

        // Enter into linking
        // ![ -> image flag true
        else if (text[index] == '!' && text[index + 1] == '[')
        {
            isImage = true;
            index++;
            continue;
        }
        else if (text[index] == '[')
        {

            // Crude implementation for images
            if (index >= 1 && text[index - 1] == '!')
            {
                isImage = true;
            }
            else
            {
                isImage = false;
            }

            index++;
            // Loop through urltext
            while (index < text.size() && text[index] != ']')
            {

                // Broken [text [text](link) case
                if (text[index] == '[')
                {
                    newText += '[' + urlText;
                    urlText = "";
                    index++;
                }

                urlText += text[index];
                index++;
            }

            // EndofUrlText
            // [urltext]
            if (text[index] == ']' && index < text.size())
            {
                index++;
                if (text[index] == '(' && index < text.size())
                {
                    index++;
                    // Continue
                    while (index < text.size() && text[index] != ')')
                    {

                        // Error [link](abc.com [link](abc.com)
                        if (text[index] == '[')
                        {
                            if (isImage)
                            {
                                newText += '!';
                            }

                            newText += '[' + urlText + "](" + url;
                            urlText = "";
                            break;
                        }
                        url += text[index];
                        index++;
                    }

                    // Complete parsing
                    if (text[index] == ')' && index < text.size())
                    {

                        if (isImage)
                        {
                            index++;
                            newText += "<img src='" + url + "' alt='" + urlText + "'>";
                        }
                        else
                        {
                            index++;
                            newText += "<a href='" + url + "'>" + urlText + "</a>";
                        }
                        url = "";
                        urlText = "";
                        isImage = false;
                        continue;
                    }

                    if (index == text.size())
                    {
                        if (isImage)
                        {
                            newText += '!';
                        }

                        newText += '[' + urlText + "](" + url;
                        continue;
                    }
                }
                else
                {
                    if (isImage)
                    {
                        newText += '!';
                    }
                    newText += '[' + urlText + ']';
                    urlText = "";
                    continue;
                }
            }
            else
            {
                if (isImage)
                {
                    newText += '!';
                }
                newText += '[' + urlText;
                urlText = "";
                continue;
            }
        }
        else
        {
            newText += text[index];
            index++;
        }
    }
    return newText;
}

// taken from abstractxan/mizi
// split to be replaced by this
vector<string> tokenizer(const std::string stringPtr, std::string delims)
{

    vector<string> tokens;
    std::size_t nextIndex, currIndex = 0;
    // Find next instance of delim
    nextIndex = stringPtr.find_first_of(delims);
    // While delims exist
    while (nextIndex != std::string::npos)
    {
        // Add token
        tokens.push_back(stringPtr.substr(currIndex, nextIndex - currIndex));
        currIndex = nextIndex + 1;
        nextIndex = stringPtr.find_first_of(delims, currIndex);
    }
    // Push last string chunk
    if (currIndex < stringPtr.size())
    {
        tokens.push_back(stringPtr.substr(currIndex, stringPtr.size() - currIndex));
    }

    return tokens;
}

// taken from abstractxan/mizi
string printVector(vector<string> vec)
{
    string ret = "{";
    for (auto item : vec)
    {
        ret += "\"" + item + "\", ";
    }
    if (!vec.empty())
    {
        // remove extra `\", `
        ret.pop_back();
        ret.pop_back();
    }
    ret += "}";
    return ret;
}

void replace(string replacent, string replacer, string &iText)
{
    size_t i = iText.find(replacent);
    if (i != string::npos)
    {
        iText.replace(i, replacent.size(), replacer);
    }
}

float findMax(std::unordered_map<int, float> imap)
{
    float ret = -1;

    for (auto val : imap)
    {
        if (val.second > ret)
            ret = val.second;
    }

    return ret;
}

string floatToString(float iNum, int precision)
{
    if (iNum == -1)
        return "Nill";
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << iNum;
    return ss.str();
}

float findAverage(std::unordered_map<int, float> imap)
{
    float total = 0;
    for (auto entry : imap)
    {
        total += entry.second;
    }
    return total / imap.size();
}
float findSD(std::unordered_map<int, float> imap)
{
    float average = findAverage(imap);
    float total = 0;
    for (auto val : imap)
    {
        total += (val.second - average) * (val.second - average);
    }
    return sqrt(total / imap.size());
}

float findDMA(std::unordered_map<int, float> imap, int num)
{
    if (imap.size() < num)
        return -1;

    int total = 0;
    int n = imap.size();
    for (int i = 0; i < num; i++)
        total += imap[n - i];

    return total / num;
}

float findUCL(std::unordered_map<int, float> imap)
{
    return findAverage(imap) + findSD(imap) * 3;
}

float findLCL(std::unordered_map<int, float> imap)
{
    return findAverage(imap) - findSD(imap) * 3;
}

// unordered_map<string, vector<string>> organiseStrayCats(vector<string> iCats)
// {
//     int idx = 0;
//     vector<string> ipaths = iCats;
//     unordered_map<string, vector<string>> ret;
//     while (idx < ipaths.size())
//     {
//         if (ipaths[idx].find(".md") == string::npos)
//         {
//             vector<string> temp;
//             for (int i = 0; i < ipaths.size(); i++)
//             {
//                 if (ipaths[i].find(ipaths[idx]))
//                 {
//                     temp.push_back(ipaths[i]);
//                 }
//                 if (ipaths[i].find(".md") != string::npos)
//                     ipaths.erase(ipaths.begin() + i);
//             }
//         }
//     }
//     return ret;
// }

int GetChildrenCount(string path)
{
    auto dirIter = std::filesystem::directory_iterator(path);
    int ChildCount = 0;

    for (auto &entry : dirIter)
    {
        ++ChildCount;
    }
    return ChildCount;
}

