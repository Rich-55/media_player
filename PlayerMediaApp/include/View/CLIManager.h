#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../View/ViewBase.h"
#include "../View/MetadataView.h"
#include "../View/ScanView.h"
#include "../View/MediaFileManagerView.h"
class CLIManager{
    private:
        std::unordered_map<std::string, std::shared_ptr<ViewBase>> views; 
        std::shared_ptr<ViewBase> currentView;
    public: 

        CLIManager();

        std::shared_ptr<ViewBase> getView(std::string);

        void switchView(std::string);

        ~CLIManager();
    
};

#endif
