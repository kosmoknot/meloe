#include <string>
#include <vector>
#include <algorithm>
#pragma once

class TemplateManager;


std::vector<std::string> split(std::string input, std::string token);
std::string toLowerCase(std::string input);
int toInt(std::string input);
float toFloat(std::string input);
std::vector<std::string> tokenizer(const std::string stringPtr,std::string delims);
std::string printVector(std::vector<std::string> vec);
std::string parseLinks(std::string text, TemplateManager *templateMgr);
void replace(string token, string replacer, string *input);