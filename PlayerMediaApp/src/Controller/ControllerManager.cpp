#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m ,CLIManager v) : model(m), view(v){}

void ControllerManager::ScanData()
{   
    std::shared_ptr<ViewBase> scan = std::make_shared<ScanView>();

    view.switchView(scan);

    MediaScannerController scanController(scan);
    this->listPathNames = scanController.scanView();
    for(size_t i = 0; i < listPathNames.size(); i++){
        std::cout << "path Name " << i + 1 << " : " << listPathNames[i] << std::endl;
    }
}

void ControllerManager::addData(){

}