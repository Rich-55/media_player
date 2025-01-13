#include "../../include/View/CLIManager.h"
CLIManager::CLIManager(){}

std::shared_ptr<ViewBase> CLIManager::getView(std::string name)  {
    auto it = views.find(name);
    if (it == views.end()) {
        if (name == "ScanView") {
            views[name] = std::make_shared<ScanView>();
        }else if (name == "MediaFileHandlerView") {
            views[name] = std::make_shared<MediaFileHandlerView>();
        } else if (name == "MediaFileManagerView") {
            views[name] = std::make_shared<MediaFileManagerView>();
        }else if (name == "PlaylistHandlerView") {
            views[name] = std::make_shared<PlaylistHandlerView>();
        }else if (name == "PlaylistManagerView") {
            views[name] = std::make_shared<PlaylistManagerView>();
        }
    }
    return views[name];
}

void CLIManager::switchView(std::string name) {
    auto it = views.find(name);
    if (it != views.end()) {
        currentView = it->second;
        currentView->showMenu();
    } else {
        std::cerr << "View not found: " << name << std::endl;
    }
}

CLIManager::~CLIManager(){}

