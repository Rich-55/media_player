#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, ViewManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaPlaylistController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistManagerController(nullptr), playerController(nullptr) 
{

}

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
                model.getPlaylistManager(),
                model.getFolderManager(), 
                scanView
            );
        }
        scannerController->handleScan(false);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// MetadataFileHandler function
void ControllerManager::metadataFileHandler() {
    try {

        std::string fileName;

        auto mediaFileHandlerView = getView("MediaFileHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");


        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                mediaFileHandlerView,
                scannerController
            );
        } 
        
        fileName = mediaFileManagerController->showAllMediaFile();
        if(fileName == ""){
            return;
        }
        
        mediaFileHandlerController = std::make_shared<MediaFileController>(
            model.getMediaFile(fileName), 
            mediaFileHandlerView
        );

        mediaFileManagerController->addMediaFileController(fileName, mediaFileHandlerController);
        
        mediaFileHandlerController->handlerMediaFile();
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
                model.getPlaylistManager(),
                model.getFolderManager(), 
                scanView
            );
        }

        auto mediaFileManagerView = getView("MediaFileManagerView");
        auto mediaFileHandlerView = getView("MediaFileHandlerView");

        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                mediaFileHandlerView,
                scannerController
            );
        }
        mediaFileManagerController->handleMediaFileManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlaylistHandler function
void ControllerManager::playlistHandler() {
    try {
        std::string playlistName;

        auto playlistManagerView = getView("PlaylistManagerView");
        auto playlistHandlerView = getView("PlaylistHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");

        if(!mediaPlaylistManagerController){
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
                model.getPlaylistManager(), 
                model.getMediaFileManager(),
                model.getFolderManager(),
                mediaFileManagerView,
                playlistManagerView,
                playlistHandlerView
            );
        }

        if (!mediaFileManagerController) {
            auto scanView = getView("ScanView");
        
            if (!scannerController) { 
                scannerController = std::make_shared<MediaScannerController>(
                    model.getMediaFileManager(), 
                    model.getPlaylistManager(),
                    model.getFolderManager(), 
                    scanView
                );
            }

            auto mediaFileManagerView = getView("MediaFileManagerView");
            auto mediaFileHandlerView = getView("MediaFileHandlerView");
            
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                mediaFileHandlerView,
                scannerController
            );
        }

        if (!model.getPlaylistManager().checkPlaylist()) {
            if (!getYesNoInput("Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ")) {
                return;
            }
            std::string name;
            std::cout << "Enter the name of the playlist (type '0' to cancel): ";
            std::getline(std::cin, name); 
            if(name == "0"){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        playlistName = mediaPlaylistManagerController->displayAllPlaylist();

        if (playlistName == "") {
            std::cout << "Returning to the main menu.\n";
            return;
        }

        mediaPlaylistController = std::make_shared<MediaPlaylistController>(
            model.getMediaFileManager(), 
            model.getFolderManager(),
            model.getPlaylist(playlistName), 
            mediaFileManagerView,
            playlistHandlerView
        );

        mediaPlaylistManagerController->addMediaPlaylistController(playlistName, mediaPlaylistController);
        
        mediaPlaylistController->handlerPlaylist();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlaylistManager function
void ControllerManager::playlistManager() {
    try {
        auto playlistManagerView = getView("PlaylistManagerView");
        auto playlistHandlerView = getView("PlaylistHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");
        if(!mediaPlaylistManagerController){
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
                model.getPlaylistManager(), 
                model.getMediaFileManager(),
                model.getFolderManager(),
                mediaFileManagerView,
                playlistManagerView,
                playlistHandlerView
            );
        }
        
        if (!model.getPlaylistManager().checkPlaylist()) {
            if (!getYesNoInput("No playlists found. Do you want to create a new playlist? (Y/N): ")) {
                return;
            }
            std::string name;
            std::cout << "Enter the name of the playlist (type '0' to cancel): ";
            std::getline(std::cin, name); 
            if(name == "0"){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        mediaPlaylistManagerController->handlerPlaylistManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::string ControllerManager::playMusicHandler() {
    
    std::string fileName;
    auto mediaFileManagerView = getView("MediaFileManagerView");
    fileName = mediaFileManagerView->displayAllMediaFileOfAudio(model.getMediaFileManager());

    if (fileName.empty()) {
        std::cerr << "Error: No file selected.\n";
        return "";
    }

    std::string filePath = model.getMediaFileManager().getMediaFile(fileName)->getPath();

    return filePath;

}

std::vector<std::string> ControllerManager::playPlaylist() {
    std::string playlistName;
    auto playlistManagerView = getView("PlaylistManagerView");
    auto playlistHandlerView = getView("PlaylistHandlerView");
    auto mediaFileManagerView = getView("MediaFileManagerView");

    if(!mediaPlaylistManagerController){
        mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
            model.getPlaylistManager(), 
            model.getMediaFileManager(),
            model.getFolderManager(),
            mediaFileManagerView,
            playlistManagerView,
            playlistHandlerView
        );
    }
    
    if (!model.getPlaylistManager().checkPlaylist()) {
        if (!getYesNoInput("No playlists found. Do you want to create a new playlist? (Y/N): ")) {
            return {};
        }
        std::string name;
        std::cout << "Enter the name of the playlist (type '0' to cancel): ";
        std::getline(std::cin, name); 
        if(name == "0"){
            return {};
        }
        mediaPlaylistManagerController->createPlaylist(name);
    }

    playlistName = mediaPlaylistManagerController->displayAllPlaylist();

    if (playlistName == "") {
        std::cout << "Returning to the main menu.\n";
        return {};
    }

    mediaPlaylistController = std::make_shared<MediaPlaylistController>(
        model.getMediaFileManager(), 
        model.getFolderManager(),
        model.getPlaylist(playlistName), 
        mediaFileManagerView,
        playlistHandlerView
    );

    
    return mediaPlaylistController->getListPathMediaFiles();
}

void ControllerManager::runApp() {
    auto mainMenuView = getView("MainMenuView");
    std::string typePlay = "noplay"; // Trạng thái ban đầu là không phát
    std::string media;
    std::vector<std::string> listMedia;
    std::shared_ptr<PlayerController> playerController; // Chuyển sang shared_ptr để truyền vào showMenuWithPlayer
    int choice;

    while (true) {
        try {
            system("clear");
            // Gọi `showMenuWithPlayer` với `playerController`
            choice = mainMenuView->showMenuWithPlayer(
                model.getMediaFileManager(),
                playerController,
                typePlay,
                media,
                listMedia
            );
            // Xử lý logic dựa trên lựa chọn
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
                case PLAY_MUSIC:
                {
                    media = playMusicHandler(); // Chọn bài nhạc để phát
                    if (!media.empty()) {
                        typePlay = "single"; // Đặt trạng thái phát nhạc đơn
                        std::vector<std::string> singleMedia = { media };
                        playerController = std::make_shared<PlayerController>(singleMedia); // Tạo player controller
                    }
                    break;
                }
                case PLAY_PLAYLIST:
                {
                    listMedia = playPlaylist(); // Chọn danh sách phát
                    if (!listMedia.empty()) {
                        typePlay = "playlist"; // Đặt trạng thái phát danh sách
                        playerController = std::make_shared<PlayerController>(listMedia); // Tạo player controller
                    }
                    break;
                }
                case EXIT_MAIN_MENU:
                    return; // Thoát chương trình
                default:
                    throw InvalidChoiceException();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}