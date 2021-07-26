#include <iostream>
#include "../include/templateManager.hpp"
#include "../include/ledgerConfig.hpp"
#include "../include/ledgerRender.hpp"
#include "../include/pageRender.hpp"
#include "../include/pageConfig.hpp"
using namespace std;

int main(){
    std::cout<<"\nStarted the Kallog"<<std::endl;
    TemplateManager* pTManager = new TemplateManager("../config/templates.md");
    PageConfig* pPConfig = new PageConfig;
    LedgerConfig* pLConfig = new LedgerConfig("../config/ledger-config.md");
    LedgerRender* pLRender = new LedgerRender("../content/ledger.md",pLConfig,pTManager,pPConfig);
    return 0;
}