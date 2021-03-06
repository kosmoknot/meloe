#include <string>
#include <unordered_map>
#include <vector>
#pragma once


struct Template
{
  std::string name;
  std::vector<std::string> args;
  std::vector<std::string> textContentList;
  std::vector<std::string> argContentList;
};

typedef std::unordered_map<std::string, Template *> TemplateMap;
class TemplateManager
{
private:
  TemplateMap *tmap;
  void templateCreatorParser(std::string templateFile);

public:
  TemplateManager(std::string templateFile);
  // Read string from md like: {{template}}
  std::string templateReaderParser(std::string mdTemplateFormat);
  Template *getTemplate(std::string templateName);
};

void parseAndSaveTemplateContent(Template *temp, std::string templateContent, bool hasMultipleLines);
std::unordered_map<std::string, std::string> *generateTemplateArgValueMap(std::vector<std::string> args);
std::string renderTemplate(Template *template_ptr, std::unordered_map<std::string, std::string> *argValMap);