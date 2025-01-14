#include "include/Model/ModelManager.h"
#include "include/View/ViewManager.h"
#include "include/Controller/ControllerManager.h"
#define MEDIA_FILE_MANAGER 1 
#define METADATA_FILE_EDIT 2
#define PLAYLIST_MANAGER 3
#define PLAYLIST_HANDLER 4
#define EXIT 0

int main() {
    ModelManager model;
    ViewManager view;
    ControllerManager controller(model, view);

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
            case MEDIA_FILE_MANAGER:
                controller.mediaFileManager();
                break;
            case METADATA_FILE_EDIT:
                controller.metadataFileHandler();
                break;
            case PLAYLIST_MANAGER:
                controller.playlistManager();
                break;
            case PLAYLIST_HANDLER:
                controller.playlistHandler();
                break;
            case EXIT:
                return 0;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
