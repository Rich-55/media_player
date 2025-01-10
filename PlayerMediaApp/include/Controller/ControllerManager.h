#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "../Model/ModelManager.h"
#include "../View/CLIManager.h"
#include "../View/ScanView.h"
#include "../Controller/MediaFileController.h"
#include "../Controller/MediaScannerController.h"

class ControllerManager{
    private:
        ModelManager model;
        CLIManager view;
        std::vector<std::string> listPathNames;
    public:
        ControllerManager(ModelManager, CLIManager);
        
        void ScanData();

        void addData();

};

#endif
