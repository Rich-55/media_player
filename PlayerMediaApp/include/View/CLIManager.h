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
        std::vector<std::shared_ptr<ViewBase>> listView;
        std::shared_ptr<ViewBase> currentView;
    public: 

        CLIManager();

        void switchView(std::shared_ptr<ViewBase>);

        void addView(std::shared_ptr<ViewBase>);

};

#endif
