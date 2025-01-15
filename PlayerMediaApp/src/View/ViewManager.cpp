#include "../../include/View/ViewManager.h"
ViewManager::ViewManager(){}

std::shared_ptr<BaseView> ViewManager::getView(std::string name)  {
    auto it = views.find(name);
    if (it == views.end()) {
        if(name == "MainMenuView"){
            views[name] = std::make_shared<MainMenuView>();
        }else if (name == "ScanView") {
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

ViewManager::~ViewManager(){}

