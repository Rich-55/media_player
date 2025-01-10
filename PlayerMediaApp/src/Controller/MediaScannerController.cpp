#include "../../include/Controller/MediaScannerController.h"

MediaScannerController::MediaScannerController(std::shared_ptr<ViewBase> view): scan(view){}

std::vector<std::string> MediaScannerController::scanView(){
    std::vector<std::string> listPathNames;
    int choice = 0;
    std::cin >> choice;

    if (choice == 1) {
        scan->scanHomeDirectory(listPathNames);
    } else if (choice == 2) {
        scan->scanUSBDevices(listPathNames);
    } else {
        std::cerr << "Invalid menu choice.\n";
    }

    return listPathNames;

}