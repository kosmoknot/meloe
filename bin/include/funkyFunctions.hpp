#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#pragma once

class TemplateManager;

std::vector<std::string> split(std::string input, std::string token);
std::string toLowerCase(std::string input);
int toInt(std::string input);
float toFloat(std::string input);
std::vector<std::string> tokenizer(const std::string stringPtr, std::string delims);
std::string printVector(std::vector<std::string> vec);
std::string parseLinks(std::string text, TemplateManager *templateMgr);
void replace(std::string token, std::string replacer, std::string &iText);
float findMax(std::unordered_map<int, float> imap);
float findAverage(std::unordered_map<int, float> imap);
float findSD(std::unordered_map<int, float> imap);
float findDMA(std::unordered_map<int, float> imap, int num);
float findUCL(std::unordered_map<int, float> imap);
float findLCL(std::unordered_map<int, float> imap);
std::string floatToString(float iNum, int resolution);