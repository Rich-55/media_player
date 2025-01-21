#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../View/BaseView.h"
#include "../View/MetadataView.h"
#include "../View/ScanView.h"
#include "../View/MediaFileManagerView.h"
#include "../View/MediaFileHandlerView.h"
#include "../View/PlaylistHandlerView.h"
#include "../View/PlaylistManagerView.h"
#include "../View/MainMenuView.h"
class ViewManager{
    private:
        std::unordered_map<std::string, std::shared_ptr<BaseView>> views; 
        std::shared_ptr<BaseView> currentView;
    public: 

        ViewManager();

        std::shared_ptr<BaseView> getView(std::string);

        ~ViewManager();
    
};

#endif
