#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, CLIManager v) : model(m), view(v),  scannerController(nullptr){}

void ControllerManager::ScanData() {
    std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

    view.switchView("ScanView");

    if (!scannerController) {
        scannerController = std::make_unique<MediaScannerController>(model.getMetadataManager(), scanView);
    }

    scannerController->scan();

}

void ControllerManager::viewAllDataAdded() {
    std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }

    view.switchView("MediaFileManagerView");

    MediaFileController fileController(model.getMetadataManager(), mediaFileManagerView);
    
    fileController.showMediaFile();
}

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