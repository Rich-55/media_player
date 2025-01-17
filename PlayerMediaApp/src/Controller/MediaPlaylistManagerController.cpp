#include "../../include/Controller/MediaPlaylistManagerController.h"
MediaPlaylistManagerController::MediaPlaylistManagerController(PlaylistManager& m, std::shared_ptr<BaseView> v) : playlistManager(m), playlistManagerView(v){}

void MediaPlaylistManagerController::createPlaylist() {
    try {
        std::string name;
        std::cout << "Enter the name of the playlist: ";
        std::cin >> name;

        if (name.empty() || name.find(' ') != std::string::npos) {
            throw InvalidPlaylistNameException("Playlist name cannot be empty or contain spaces. Provided: " + name);
        }

        if (!std::all_of(name.begin(), name.end(), [](unsigned char c) {
                return std::isalnum(c); // Kiểm tra chữ cái hoặc số
            })) {
            throw InvalidPlaylistNameException("Playlist name must contain only alphabetic or numeric characters. Provided: " + name);
        }

        if (playlistManager.checkPlaylistName(name)) {
            throw DuplicatePlaylistException(name);
        }

        playlistManager.addPlaylist(std::make_shared<Playlist>(name));
        std::cout << "Playlist '" << name << "' created successfully.\n";
    } catch (const PlaylistManagerException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void MediaPlaylistManagerController::deletePlaylist() {
    try {
        std::string name;
        std::cout << "Enter the name playlist to delete: ";
        std::cin >> name;

        if (name.empty() || name.find(' ') != std::string::npos || !std::all_of(name.begin(), name.end(), ::isalpha)) {
            throw InvalidPlaylistNameException("Playlist name must contain only alphabetic characters and no spaces. Provided: " + name);
        }

        if (!playlistManager.checkPlaylistName(name)) {
            throw PlaylistNotFoundException(name);
        }

        playlistManager.deletePlaylist(name);
        std::cout << "Playlist '" << name << "' deleted successfully.\n";
    } catch (const PlaylistManagerException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MediaPlaylistManagerController::displayAllPlaylist()
{
    playlistManagerView->displayAllPlaylist(playlistManager);
}

void MediaPlaylistManagerController::handlerPlaylistManager() {
    int choice;
    while (true) {
        try {
            choice = playlistManagerView->showMenu();

            switch (choice) {
            case CREATE_PLAYLIST:
                createPlaylist();
                break;
            case DELETE_PLAYLIST:
                deletePlaylist();
                break;
            case DISPLAY_ALL_PLAYLIST:
                displayAllPlaylist();
                break;
            case EXIT_MENU_PLAYLIST_MANAGER:
                return;
            default:
                throw InvalidChoiceException();
            }
        } catch (const PlaylistManagerException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }catch (const InvalidChoiceException &e) {
            std::cerr << "Error: " << e.what() << '\n'; 
        }
    }
}
