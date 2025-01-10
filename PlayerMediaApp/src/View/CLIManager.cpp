#include "../../include/View/CLIManager.h"
CLIManager::CLIManager(){}

void CLIManager::addView(const std::string &name, std::shared_ptr<ViewBase> view) {
    views[name] = view;
}

void CLIManager::switchView(std::string name){
    auto view = getView(name);
    if (view) {
        currentView = view;
        currentView->showMenu(); 
        std::cerr << "View not found: " << name << '\n';
    }
}

std::shared_ptr<ViewBase> CLIManager::getView(std::string name) {
    if (views.find(name) == views.end()) {
        if (name == "ScanView") {
            views[name] = std::make_shared<ScanView>();
        } else if (name == "MetadataView") {
            views[name] = std::make_shared<MetadataView>();
        // } else if (name == "MediaFileManagerView") {
        //     views[name] = std::make_shared<MediaFileManagerView>();
        } else {
            std::cerr << "Unknown View: " << name << std::endl;
            return nullptr;
        }
    }
    return views[name];
}