#include "../include/funkyFunctions.hpp"

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

string toLowerCase(string input)
{
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

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