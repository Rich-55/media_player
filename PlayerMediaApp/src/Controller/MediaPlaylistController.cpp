#include "../../include/Controller/MediaPlaylistController.h"

MediaPlaylistController::MediaPlaylistController(MediaFileManager& m,std::shared_ptr<Playlist> p, std::shared_ptr<BaseView> v) : mediaFileManager(m), playlist(p), playlistHandlerView(v){}

void MediaPlaylistController::addMediaFile(std::shared_ptr<MediaFile> mediaFile) {
    try {
        if (playlist->checkMediaFile(mediaFile->getName())) {
            throw DuplicateMediaFileException(mediaFile->getName());
        }
        playlist->addMediaFile(mediaFile);
        std::cout << "File '" << mediaFile->getName() << "' added to playlist successfully.\n";
    } catch (const PlaylistHandlerException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MediaPlaylistController::deleteMediaFile() {
    try {
        std::cout << "Enter the name of the file you want to delete: ";
        std::string fileName;
        std::cin >> fileName;

        if (fileName.empty() || fileName.find(' ') != std::string::npos) {
            throw InvalidFileNameException(fileName);
        }

        if (!playlist->checkMediaFile(fileName)) {
            throw FileNotFoundInPlaylistException(fileName);
        }

        playlist->deleteMediaFile(fileName);
        std::cout << "File '" << fileName << "' deleted from playlist successfully.\n";
    } catch (const PlaylistHandlerException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void MediaPlaylistController::displayAllMediaFiles()
{
    playlistHandlerView->displayAllMediaFileInPlaylist(playlist);
}

void MediaPlaylistController::handlerPlaylist() {
    int choice;
    while (true) {
        try {
            choice = playlistHandlerView->showMenu();

            switch (choice) {
            case ADD_MEDIA_FILE_TO_PLAYLIST:
            {
                std::cout << "Enter the name of the file you want to add: ";
                std::string fileName;
                std::cin >> fileName;

                if (fileName.empty() || fileName.find(' ') != std::string::npos) {
                    throw InvalidFileNameException(fileName);
                }

                std::shared_ptr<MediaFile> mediaFile = mediaFileManager.getMediaFile(fileName);
                if (mediaFile == nullptr) {
                    throw FileNotFoundInPlaylistException(fileName);
                }

                addMediaFile(mediaFile);
            }
                break;
            case DELETE_MEDIA_FILE_FROM_PLAYLIST:
                deleteMediaFile();
                break;
            case DISPLAY_ALL_MEDIA_FILE_FROM_PLAYLIST:
                displayAllMediaFiles();
                break;
            case EXIT_MENU_PLAYLIST_HANDLER:
                return;
            default:
                throw InvalidChoiceException();
            }
        } catch (const PlaylistHandlerException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }catch (const InvalidChoiceException &e) {
            std::cerr << "Error: " << e.what() << '\n'; 
        }
    }
}
