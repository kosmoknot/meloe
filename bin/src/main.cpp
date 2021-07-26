#include <iostream>
#include "../include/templateConfig.hpp"
#include "../include/ledgerConfig.hpp"
#include "../include/ledgerRender.hpp"
#include "../include/pageRender.hpp"
#include "../include/pageConfig.hpp"
using namespace std;

int main(){
    std::cout<<"\nStarted the Kallog"<<std::endl;
    TemplateConfig* pTConfig = new TemplateConfig;
    PageConfig* pPConfig = new PageConfig;
    LedgerConfig* pLConfig = new LedgerConfig("../config/ledger-config.md");
    LedgerRender* pLRender = new LedgerRender("../content/ledger.md",pLConfig,pTConfig,pPConfig);
    return 0;
}