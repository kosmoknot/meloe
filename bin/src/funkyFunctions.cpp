#include "../include/funkyFunctions.hpp"
#include <iostream>

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

//orignal function replaced with same function from mizi
// string toLowerCase(string input)
// {
//     transform(input.begin(), input.end(), input.begin(), ::tolower);
//     return input;
// }

string toUpperCase(string input)
{
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}

int toInt(string input){
    if(input.find_first_not_of("1234567890 ")==string::npos){
        return stoi(input);
    }
    return -1;
}

float toFloat(string input){
    if(input.find_first_not_of(".1234567890 ")==string::npos){
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

//taken from mizi
string toLowerCase(string text)
{
    string newtext = "";
    for (unsigned int i = 0; i < text.size(); i++)
    {
        if (text[i] == '\0')
            break;
        else if (text[i] == ' ')
            newtext += '_';
        else if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z')) //alphabets
            newtext += getLower(text[i]);
        else if (text[i] >= '0' && text[i] <= '9')
            newtext += getLower(text[i]);
    }
    return newtext;
}
// Prints errorline
void printError(int linenumber, string text) {
  cout << "Error at line " << linenumber << ". " << text << endl;
}


//taken from abstractxan/mizi
// {{tags}} -> <a class='tag' href='tags.html'> {{tags}}</a>
//  [urlText](url) ->  <a href='url'>urlText</a>
// ![altText](image) -> <img src='' alt=''>

string parseLinks(string text, string path)
{
    path = "";
    string label = text;
    unsigned int index = 0;
    string newText = "";
    string urlText = "";
    string url = "";
    bool isImage = false;
    bool isTag = false;

    for (; index < text.size();)
    {

        // Check if tag
        if (text[index] == '{' && text[index + 1] == '{' && !isTag)
        {
            isTag = true;
            index += 2;
            continue;
        }
        
        // If tag
        if (isTag == true)
        {
            string tag = "";
            while (index < text.size())
            {
                if(text[index] == '}' && text[index+1] == '}'){
                    string filename = toLowerCase(tag);
                    string filepath = filename + ".html";
                    newText += "<a class='tag' href='" + filepath + "'>{{" + tag + "}}</a>";
                    isTag = false;
                    break;
                }
                tag += text[index];
                index += 1;
            }

            //if bad tags
            if(index == text.size() && isTag == true){
                newText += "{{" + tag;
                isTag = false;
            }

            index += 2; // Increment index to new text;
            continue;
        }



        // Enter into linking
        // ![ -> image flag true
        if (text[index] == '!' && text[index + 1] == '[')
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
                            if (isImage){
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
                        if (isImage){
                                newText += '!';
                        }

                        newText += '[' + urlText + "](" + url;
                        continue;
                    }
                }
                else
                {
                    if (isImage){
                        newText += '!';
                    }
                    newText += '[' + urlText + ']';
                    urlText = "";
                    continue;
                }
            }
            else
            {
                if (isImage){
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

//taken from abstractxan/mizi
//split to be replaced by this
vector<string> tokenizer(const std::string stringPtr, const std::string &delims = " ")
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

//taken from abstractxan/mizi
string printVector(vector<string> vec)
{
    string ret = "{";
    for (auto item : vec)
    {
        ret += "\"" + item + "\", ";
    }
    if (!vec.empty())
    {
        //remove extra `\", `
        ret.pop_back();
        ret.pop_back();
    }
    ret += "}";
    return ret;
}