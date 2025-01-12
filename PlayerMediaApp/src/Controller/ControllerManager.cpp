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

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        fileController.showAllMediaFile();
    } else if (choice == 2) {
        fileController.showAllAudioFile();
    } else if (choice == 3) {
        return;
    } else if (choice == 4) {
        std::cout << "Exiting...\n";
        return;
    } else {
        std::cout << "Invalid option. Try again.\n";
    }

}

