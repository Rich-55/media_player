#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../View/ViewBase.h"
#include "../View/MetadataView.h"
#include "../View/ScanView.h"
class CLIManager{
    private:
        std::unordered_map<std::string, std::shared_ptr<ViewBase>> views; 
        std::shared_ptr<ViewBase> currentView;
    public: 
        template <typename T>
        std::shared_ptr<T> getViewAs(const std::string &name) {
            auto view = getView(name);
            return std::dynamic_pointer_cast<T>(view);
        }
    
        CLIManager();

        std::shared_ptr<ViewBase> getView(std::string);

        void switchView(std::string);

        void addView(const std::string &name, std::shared_ptr<ViewBase> view);



};

#endif
