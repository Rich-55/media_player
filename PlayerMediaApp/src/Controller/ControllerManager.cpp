#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, ViewManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistController(nullptr), mediaPlaylistManagerController(nullptr) {}

void ControllerManager::ScanData() 
{   
    std::shared_ptr<BaseView> scanView = view.getView("ScanView");

    if (!scanView) {
        std::cerr << "Error: ScanView not found or failed to initialize.\n";
        return;
    }
    if (!scannerController) {
       scannerController = std::make_shared<MediaScannerController>(model.getMediaFileManager(), model.getFolderManager(), scanView);
    }
    scannerController->handleScan(false);
}

void ControllerManager::mediaFileManager() 
{
    std::shared_ptr<BaseView> scanView = view.getView("ScanView");

    if (!scanView) {
        std::cerr << "Error: ScanView not found or failed to initialize.\n";
        return;
    }
    if (!scannerController) {
       scannerController = std::make_shared<MediaScannerController>(model.getMediaFileManager(), model.getFolderManager(), scanView);
    }

    std::shared_ptr<BaseView> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }
    if(!mediaFileManagerController){
        mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMediaFileManager(), mediaFileManagerView, scannerController);
    }
    mediaFileManagerController->handleMediaFileManager();
}

void ControllerManager::metadataFileHandler()
{
    std::shared_ptr<BaseView> mediaFileHandlerView = view.getView("MediaFileHandlerView");

    if (!mediaFileHandlerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }

    std::shared_ptr<BaseView> mediaFileManagerView = view.getView("MediaFileManagerView");
    
    if (!mediaFileManagerView) {
        std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
        return;
    }
    if(!mediaFileManagerController){
        mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMediaFileManager(), mediaFileManagerView, scannerController);
        mediaFileManagerController->showAllMediaFile();
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

    mediaFileHandlerController->handlerMediaFile();
}

void ControllerManager::playlistManager()
{
    std::shared_ptr<BaseView> playlistManagerView = view.getView("PlaylistManagerView");

    if (!playlistManagerView) {
        std::cerr << "Error: PlaylistManagerView not found or failed to initialize.\n";
        return;
    }

    if (!mediaPlaylistManagerController) {
        mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(model.getPlaylistManager(), playlistManagerView);
    }

    if (!model.getPlaylistManager().checkPlaylist()) {
        std::cout << "No playlists found. Do you want to create a new playlist? (Y/N): ";
        char choice;
        std::cin >> choice;

        if (choice == 'N' || choice == 'n') {
            return; 
        }

        mediaPlaylistManagerController->createPlaylist(); 
    }
    mediaPlaylistManagerController->handlerPlaylistManager();
}

void ControllerManager::playlistHandler() {
    std::shared_ptr<BaseView> playlistManagerView = view.getView("PlaylistManagerView");

    if (!playlistManagerView) {
        std::cerr << "Error: PlaylistHandlerView not found or failed to initialize.\n";
        return;
    }

    if (!mediaPlaylistManagerController) {
        mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(model.getPlaylistManager(), playlistManagerView);
    }

    if (!model.getPlaylistManager().checkPlaylist()) {
        std::cout << "Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ";
        
        std::string choice;
        std::cin.ignore();
        getline(std::cin, choice);

        if (choice == "N" || choice == "n") {
            return; 
        }

        mediaPlaylistManagerController->createPlaylist(); 
    }

    mediaPlaylistManagerController->displayAllPlaylist();

    std::shared_ptr<BaseView> playlistHandlerView = view.getView("PlaylistHandlerView");

    if (!playlistHandlerView) {
        std::cerr << "Error: PlaylistHandlerView not found or failed to initialize.\n";
        return;
    }

    std::cout << "Enter the name of the playlist you want to edit: ";
    std::string playlistName;
    std::cin >> playlistName;

    if (!model.getPlaylistManager().checkPlaylistName(playlistName)) {
        std::cerr << "Playlist not found!\n";
        return;
    }

    if (!mediaPlaylistController) {
        mediaPlaylistController = std::make_unique<MediaPlaylistController>(model.getMediaFileManager(), model.getPlaylist(playlistName), playlistHandlerView);
    }

    mediaPlaylistController->handlerPlaylist();
}
