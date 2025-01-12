#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, CLIManager v) : model(m), view(v),  scannerController(nullptr), mediaFileController(nullptr){}

void ControllerManager::ScanData() {
    std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

    if (!scanView) {
        std::cerr << "Error: ScanView not found or failed to initialize.\n";
        return;
    }
    if (!scannerController) {
        scannerController = std::make_unique<MediaScannerController>(model.getMetadataManager(), scanView);
    }

    std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }
    if(!mediaFileController){
        mediaFileController = std::make_unique<MediaFileController>(model.getMetadataManager(), mediaFileManagerView);
    }

    while(true){
        view.switchView("ScanView");
        int choice; std::cin >> choice;
        switch (choice)
        {
        case 1:
            scannerController->scanHomeDirectory();
            mediaFileController->addData(scannerController->getListPaths());
            break;
        case 2:
            scannerController->scanUSBDevices();
            mediaFileController->addData(scannerController->getListPaths());
            break;
        case 0:
            return;
        default:
            break;
        }
    }

}

void ControllerManager::metadataFileHandler() {
    std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }
    if(!mediaFileController){
        mediaFileController = std::make_unique<MediaFileController>(model.getMetadataManager(), mediaFileManagerView);
    }

    while(true){
        view.switchView("MediaFileManagerView");
        int choice; std::cin >> choice;
        switch (choice)
        {
        case 1:
            mediaFileController->showAllMediaFile();
            break;
        case 2:
            mediaFileController->showAllMediaFileOfAudio();
            break;
        case 3:
            mediaFileController->showAllMediaFileOfVideo();
            break;    
        case 0:
            return;
        default:
            break;
        }
    }

    
}


