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
    public:
        ControllerManager(ModelManager, CLIManager);
        
        void ScanData();

        void viewAllDataAdded();

};

#endif
