#include <iostream>
#include "../include/templateManager.hpp"
#include "../include/ledgerConfig.hpp"
#include "../include/ledgerRender.hpp"
#include "../include/pageRender.hpp"
using namespace std;

int main()
{
    std::cout << "\nStarted the MaLoE" << std::endl;

    TemplateManager *pTemplateManager = new TemplateManager("../config/templates.md");
    std::cout << "\nStarted the template manager" << std::endl;

    pageManager *pPageManager = new pageManager(pTemplateManager);
    std::cout << "\nStarted the page manager" << std::endl;

    LedgerConfig *pLConfig = new LedgerConfig("../config/ledger-config.md");
    std::cout << "\nProcessed ledger config " << std::endl;

    LedgerRender *pLRender = new LedgerRender("../content/ledger.md", pLConfig, pTemplateManager, pPageManager);
    std::cout << "\nRendered the ledger" << std::endl;

    pageManager->RenderPages();
    std::cout << "\nRendered stray-cats, wiki and home" << std::endl;

    return 0;
}