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
            throw std::runtime_error("Invalid input. Please enter Y/y or N/n.");
            continue;
        }

        if (choice == "Y" || choice == "y") return true;
        if (choice == "N" || choice == "n") return false;

        throw std::runtime_error("Invalid input. Please enter Y/y or N/n.");
    }
}

// ScanData function: Scan data from the folder in directory or USB
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

// MetadataFileHandler function: Handle metadata file (add, delete, edit)
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

// MediaFileManager function: Handle media file (add, delete, edit, view)
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

// PlaylistHandler function: Handle playlist (add media, delete media, view)
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

        playlistName = mediaPlaylistManagerController->displayAllPlaylist();

        if (playlistName == "") {
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

// PlaylistManager function: Handle playlist (create, delete, view)
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
            std::string name;
            name = playlistManagerView->showMenuCreatePlaylist();
            if(name == ""){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        mediaPlaylistManagerController->handlerPlaylistManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlayMusicHandler function: Play single music
std::string ControllerManager::playMusicHandler() {
    try {
        auto mediaFileManagerView = getView("MediaFileManagerView");
        std::string fileName;

        fileName = mediaFileManagerView->displayAllMediaFileOfAudio(model.getMediaFileManager());

        if (fileName.empty()) {
            return "";
        }

        std::string filePath = model.getMediaFileManager().getMediaFile(fileName)->getPath();

        return filePath;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
        return "";
    }
}

// PlayPlaylist function: Play playlist
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
    if(model.getFolderManager().getListFolderDirectory().empty()  && model.getFolderManager().getListFolderUSB().empty()){return;}
    auto mainMenuView = getView("MainMenuView");
    std::string typePlay = "noplay"; 
    std::string media;
    std::vector<std::string> listMedia;
    std::shared_ptr<PlayerController> playerController; 
    int choice;

    while (true) {
        try {
            choice = mainMenuView->showMenuWithPlayer(
                model.getMediaFileManager(),
                playerController,
                typePlay,
                media,
                listMedia
            );
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
                    media = playMusicHandler(); 
                    if (!media.empty()) {
                        typePlay = "single"; 
                        std::vector<std::string> singleMedia = { media };
                        playerController = std::make_shared<PlayerController>(singleMedia); // Tạo player controller
                    }
                    break;
                }
                case PLAY_PLAYLIST:
                {
                    listMedia = playPlaylist(); 
                    if (!listMedia.empty()) {
                        typePlay = "playlist"; 
                        playerController = std::make_shared<PlayerController>(listMedia); 
                    }else{
                        typePlay = "noplay";
                    }
                    break;
                }
                case EXIT_MAIN_MENU:
                    return; 
                default:
                    throw InvalidChoiceException();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}