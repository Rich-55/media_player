#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "../Model/ModelManager.h"
#include "../View/CLIManager.h"
#include "../View/ScanView.h"
#include "../View/MediaFileManagerView.h"
#include "../Controller/MediaFileController.h"
#include "../Controller/MediaScannerController.h"


class ControllerManager{
    private:
        ModelManager model;
        CLIManager view;
        std::unique_ptr<MediaScannerController> scannerController;
    public:
        ControllerManager(ModelManager, CLIManager);
        
        void ScanData();

        void viewAllDataAdded();

};

#endif