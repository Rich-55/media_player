#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m ,CLIManager v) : model(m), view(v){}

void ControllerManager::ScanData()
{   
    std::shared_ptr<ViewBase> scan = std::make_shared<ScanView>();

    view.switchView(scan);

    MediaScannerController scanController(scan, model.getMetadataManager());

    scanController.scanView();
}

void ControllerManager::viewAllDataAdded(){
    
}