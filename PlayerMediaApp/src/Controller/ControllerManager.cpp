#include "../../include/Controller/ControllerManager.h"
ControllerManager::ControllerManager(ModelManager m, ViewManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistController(nullptr), mediaPlaylistManagerController(nullptr) {}

std::shared_ptr<BaseView> ControllerManager::getView(const std::string& viewName) {
    auto viewPtr = view.getView(viewName);
    if (!viewPtr) {
        throw std::runtime_error("Error: " + viewName + " not found or failed to initialize.");
    }
    return viewPtr;
}

bool ControllerManager::getYesNoInput(const std::string& prompt) {
    std::string choice;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please enter Y/y or N/n.\n";
            continue;
        }

        if (choice == "Y" || choice == "y") return true;
        if (choice == "N" || choice == "n") return false;

        std::cerr << "Invalid input. Please enter Y/y or N/n.\n";
    }
}

// ScanData function
void ControllerManager::ScanData() {
    try {
        auto scanView = getView("ScanView");

        if (!scannerController) {
            scannerController = std::make_shared<MediaScannerController>(
                model.getMediaFileManager(), 
                model.getFolderManager(), 
                scanView
            );
        }
        scannerController->handleScan(false);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// MediaFileManager function
void ControllerManager::mediaFileManager() {
    try {
        auto scanView = getView("ScanView");
        if (!scannerController) {
            scannerController = std::make_shared<MediaScannerController>(
                model.getMediaFileManager(), 
                model.getFolderManager(), 
                scanView
            );
        }

        auto mediaFileManagerView = getView("MediaFileManagerView");
        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                scannerController
            );
        }
        mediaFileManagerController->handleMediaFileManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// MetadataFileHandler function
void ControllerManager::metadataFileHandler() {
    try {
        auto mediaFileHandlerView = getView("MediaFileHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");

        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                scannerController
            );
            
        }
        mediaFileManagerController->showAllMediaFile();
        std::string fileName;
        while (true) {
            std::cout << "Enter the name of the file you want to edit (or press 0 to return): ";
            std::cin >> fileName;

            if (fileName == "0") {
                std::cout << "Returning to the main menu.\n";
                return;
            }

            if (std::cin.fail() || model.getMediaFile(fileName) == nullptr) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid or not found. Please try again.\n";
                continue;
            }

            break;
        }

        if (!mediaFileHandlerController) {
            mediaFileHandlerController = std::make_unique<MediaFileController>(
                model.getMediaFile(fileName), 
                mediaFileHandlerView
            );
        }
        mediaFileHandlerController->handlerMediaFile();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlaylistManager function
void ControllerManager::playlistManager() {
    try {
        auto playlistManagerView = getView("PlaylistManagerView");

        if (!mediaPlaylistManagerController) {
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
                model.getPlaylistManager(), 
                playlistManagerView
            );
        }

        if (!model.getPlaylistManager().checkPlaylist()) {
            if (!getYesNoInput("No playlists found. Do you want to create a new playlist? (Y/N): ")) {
                return;
            }
            mediaPlaylistManagerController->createPlaylist();
        }
        mediaPlaylistManagerController->handlerPlaylistManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlaylistHandler function
void ControllerManager::playlistHandler() {
    try {
        auto playlistManagerView = getView("PlaylistManagerView");

        if (!mediaPlaylistManagerController) {
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
                model.getPlaylistManager(), 
                playlistManagerView
            );
        }

        if (!model.getPlaylistManager().checkPlaylist()) {
            if (!getYesNoInput("Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ")) {
                return;
            }
            mediaPlaylistManagerController->createPlaylist();
        }

        mediaPlaylistManagerController->displayAllPlaylist();

        auto playlistHandlerView = getView("PlaylistHandlerView");

        std::string playlistName;
        while (true) {
            std::cout << "Enter the name of the playlist you want to edit (or press 0 to return): ";
            std::cin >> playlistName;

            if (playlistName == "0") {
                std::cout << "Returning to the main menu.\n";
                return;
            }

            if (model.getPlaylistManager().checkPlaylistName(playlistName)) {
                break;
            }

            std::cerr << "Playlist not found. Please enter a valid playlist name.\n";
        }

        if (!mediaPlaylistController) {
            mediaPlaylistController = std::make_unique<MediaPlaylistController>(
                model.getMediaFileManager(), 
                model.getPlaylist(playlistName), 
                playlistHandlerView
            );
        }
        mediaPlaylistController->handlerPlaylist();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ControllerManager::runApp(){
    int choice;

    auto mainMenuView = getView("MainMenuView");

    while (true) {
        try{
            mainMenuView->showMenu();

             if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Bỏ qua dữ liệu nhập không hợp lệ
                std::cerr << "Invalid input. Please enter a number.\n";
                continue;
            }

            switch (choice) {
                case METADATA_FILE_HANDLER:
                    metadataFileHandler();
                    break;
                case MEDIA_FILE_MANAGER:
                    mediaFileManager();
                    break;
                case PLAYLIST_MANAGER:
                    playlistManager();
                    break;
                case PLAYLIST_HANDLER:
                    playlistHandler();
                    break;
                case EXIT_MAIN_MENU:
                    return;
                default:
                    throw InvalidChoiceException();
            }
        }catch(const MainMenuException& e){
                std::cerr << "Error: " << e.what() << std::endl;
        }catch (const InvalidChoiceException &e) {
            std::cerr << "Error: " << e.what() << '\n'; 
        }
    }
}