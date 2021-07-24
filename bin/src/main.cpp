#include <iostream>
#include "../include/template.hpp"
#include "../include/ledger.hpp"
using namespace std;

int main(){
    std::cout<<"\nStarted the Kallog"<<std::endl;
    Template* pTemplate = new Template;
    Config* pConfig = new Config;
    Ledger* pLedger = new Ledger("../config/ledger-config.md","..content/ledger.md",pConfig,pTemplate);
    pLedger->render();
    return 0;
}