#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, CLIManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaFileManagerController(nullptr) {}

void ControllerManager::ScanData() 
{
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
    if(!mediaFileManagerController){
        mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
    }

    while(true){
        view.switchView("ScanView");
        int choice; std::cin >> choice;
        switch (choice)
        {
        case 1:
            scannerController->scanHomeDirectory();
            mediaFileManagerController->addData(scannerController->getListPaths());
            break;
        case 2:
            scannerController->scanUSBDevices();
            mediaFileManagerController->addData(scannerController->getListPaths());
            break;
        case 0:
            return;
        default:
            break;
        }
    }

}

void ControllerManager::mediaFileManager() 
{
    std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }
    if(!mediaFileManagerController){
        mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
    }
    int choice; 
    while(true){
        view.switchView("MediaFileManagerView");
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            {
            std::cout << "Enter the name of the file you want to delete: ";
            std::string fileName;
            std::cin >> fileName;
            mediaFileManagerController->deleteData(fileName);
            break;
            }
        case 2:
            mediaFileManagerController->showAllMediaFile();
            break;
        case 3:
            mediaFileManagerController->showAllMediaFileOfAudio();
            break;
        case 4:
            mediaFileManagerController->showAllMediaFileOfVideo();
            break;    
        case 0:
            return;
        default:
            break;
        }
    }
}

void ControllerManager::metadataFileHandler()
{
    std::shared_ptr<ViewBase> mediaFileHandlerView = view.getView("MediaFileHandlerView");

    if (!mediaFileHandlerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }

    std::cout << "Enter the name of the file you want to edit: ";
    std::string fileName;
    std::cin >> fileName;

    if(!mediaFileHandlerController){
        if(model.getMediaFile(fileName) == nullptr){
            std::cerr << "File not found!";
            return;
        }
        mediaFileHandlerController = std::make_unique<MediaFileController>(model.getMediaFile(fileName), mediaFileHandlerView);
    }
    int choice;
    while(true){
        mediaFileHandlerController->getDetailMediaFile();
        view.switchView("MediaFileHandlerView");
         std::cin >> choice;
        switch (choice)
        {
        case 1:
            mediaFileHandlerController->addMetadata();
            break;
        case 2:
            mediaFileHandlerController->editMetadata();
            break;
        case 3:
            mediaFileHandlerController->deleteMetadata();
            break;    
        case 0:
            return;
        default:
            break;
        }
    }

}

void ControllerManager::playlistManager(){

}