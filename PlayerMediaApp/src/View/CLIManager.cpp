#include "../../include/View/CLIManager.h"
CLIManager::CLIManager(){}

void CLIManager::switchView(std::shared_ptr<ViewBase> viewBase){
    this->currentView = viewBase;
    currentView->showMenu();
}

void CLIManager::addView(std::shared_ptr<ViewBase> view) {
        listView.push_back(view);
}