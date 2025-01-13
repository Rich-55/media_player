#include "../../include/View/CLIManager.h"

CLIManager::CLIManager(){}

std::shared_ptr<ViewBase> CLIManager::getView(std::string name)  {
    auto it = views.find(name);
    if (it == views.end()) {
        if (name == "ScanView") {
            views[name] = std::make_shared<ScanView>();
        } else if (name == "MediaFileManagerView") {
            views[name] = std::make_shared<MediaFileManagerView>();
        } else if (name == "MediaFileHandlerView") {
            views[name] = std::make_shared<MediaFileHandlerView>();
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

void CLIManager::prompt(std::string s) {
    std::cout << s;
}

int CLIManager::getInput() {
    int i;
    std::cin >> i;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return i;
}

std::string CLIManager::getString() {
    std::string s;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::getline(std::cin, s);

    return s;
}

CLIManager::~CLIManager(){}
