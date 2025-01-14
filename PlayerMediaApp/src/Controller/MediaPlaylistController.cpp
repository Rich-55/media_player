#include "../../include/Controller/MediaPlaylistController.h"

MediaPlaylistController::MediaPlaylistController(MediaFileManager& m,std::shared_ptr<Playlist> p, std::shared_ptr<BaseView> v) : mediaFileManager(m), playlist(p), playlistHandlerView(v){}

void MediaPlaylistController::addMediaFile(std::shared_ptr<MediaFile> mediaFile)
{
    playlist->addMediaFile(mediaFile);
}

void MediaPlaylistController::deleteMediaFile()
{
    std::cout << "Enter the name of the file you want to delete: ";
    std::string fileName;
    std::cin >> fileName;
    playlist->deleteMediaFile(fileName);
}

void MediaPlaylistController::displayAllMediaFiles()
{
    playlistHandlerView->displayAllMediaFileInPlaylist(playlist);
}

void MediaPlaylistController::handlerPlaylist()
{
    int choice;
    while (true) {
        playlistHandlerView->showMenu();
        std::cin >> choice;
        switch (choice) {
        case 1:
        {
            std::cout << "Enter the name of the file you want to add: ";
            std::string fileName;
            std::cin >> fileName;
            std::shared_ptr<MediaFile> mediaFile = mediaFileManager.getMediaFile(fileName);
            if(mediaFile == nullptr){
                std::cerr << "File not found!\n";
                return;
            }
            addMediaFile(mediaFile);
        }
            
            break;
        case 2:
            deleteMediaFile();
            break;
        case 3:
            displayAllMediaFiles();
            break;
        case 0:
            return; 
        default:
            std::cerr << "Invalid choice. Please try again.\n";
            break;
        }
    }
}