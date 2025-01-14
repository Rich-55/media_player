#include "include/Model/ModelManager.h"
#include "include/View/ViewManager.h"
#include "include/Controller/ControllerManager.h"
#define MEDIAFILEMANAGER 1 

int main() {
    ModelManager model;
    ViewManager view;
    ControllerManager controller(model, view);

    // Init data
    controller.ScanData();

    int choice;
    while (true) {
        std::cout << "============Main Menu==========" << std::endl;
        std::cout << "1. MediaFile Manager" << std::endl;
        std::cout << "2. Metadata File Edit" << std::endl;
        std::cout << "3. Playlist Manager" << std::endl;
        std::cout << "4. Playlist Handler" << std::endl;
        
        std::cout << "0. Exit" << std::endl;

        std::cin >> choice;

        switch (choice) {
            case MEDIAFILEMANAGER:
                controller.mediaFileManager();
                break;
            case 2:
                controller.metadataFileHandler();
                break;
            case 3:
                controller.playlistManager();
                break;
            case 4:
                controller.playlistHandler();
                break;
            case 0:
                return 0;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
