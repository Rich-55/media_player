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

void ControllerManager::MediaFileControll() {
    std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }

    view.switchView("MediaFileManagerView");

    // MediaFileController fileController(model.getMetadataManager(), mediaFileManagerView);

    std::shared_ptr<MediaFileController> fileController = std::make_shared<MediaFileController>(model.getMetadataManager(), mediaFileManagerView);

    int choice = view.getInput();

    if (choice == 1) {
        fileController->showAllMediaFile();

    } else if (choice == 2) {
        fileController->showAllAudioFile();

    } else if (choice == 3) {
        // fileController->showAllVideoFile();
        return;
    } else if (choice == 0) {
        view.prompt("Exiting...");
        return;
    } else {
        view.prompt("Invalid option. Try again.");
    }
}

void ControllerManager::MediaFileHandler() {
    std::shared_ptr<ViewBase> mediaFileHandlerView = view.getView("MediaFileHandlerView");
    
    if (!mediaFileHandlerView) {
        std::cerr << "Error: MediaFileHandlerView not found or failed to initialize.\n";
        return;
    }

    view.switchView("MediaFileHandlerView");

    std::shared_ptr<MediaFileController> fileController = std::make_shared<MediaFileController>(model.getMetadataManager(), mediaFileHandlerView);

    int choice = view.getInput();

    if (choice == 1) {
        // fileController->addMediaFile();
        return;

    } else if (choice == 2) {
        view.prompt("Input file name to edit: ");
        fileController->editMediaFile(view.getString());

    } else if (choice == 3) {
        view.prompt("Input file name to remove: ");
        fileController->removeMediaFile(view.getString());

    } else if (choice == 0) {
        // std::cout << "Go Home blah blah...\n";
        return;
    } else {
        view.prompt("Invalid option. Try again.");
    }
}
