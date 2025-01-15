#include "../../include/Controller/MediaPlaylistManagerController.h"
MediaPlaylistManagerController::MediaPlaylistManagerController(PlaylistManager& m, std::shared_ptr<BaseView> v) : playlistManager(m), playlistManagerView(v){}

void MediaPlaylistManagerController::createPlaylist() {
    try {
        std::string name;
        std::cout << "Enter the name of the playlist: ";
        std::cin >> name;

        if (name.empty() || name.find(' ') != std::string::npos) {
            throw InvalidPlaylistNameException(name);
        }

        if (!std::all_of(name.begin(), name.end(), ::isalpha)) {
            throw InvalidPlaylistNameException("Playlist name must contain only alphabetic characters. Provided: " + name);
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
            playlistManagerView->showMenu();

            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InvalidPlaylistNameException("Invalid input. Expected a number.");
            }

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
