#include "../../include/Controller/MediaPlaylistManagerController.h"
MediaPlaylistManagerController::MediaPlaylistManagerController(PlaylistManager& m, std::shared_ptr<BaseView> v) : playlistManager(m), playlistManagerView(v){}

void MediaPlaylistManagerController::createPlaylist()
{
    std::string name;
    std::cout << "Enter the name of the playlist: ";
    std::cin >> name;

    playlistManager.addPlaylist(std::make_shared<Playlist>(name));
}

void MediaPlaylistManagerController::deletePlaylist()
{
    std::string name;
    std::cout << "Enter the name playlist to delete: ";
    std::cin >> name;
    playlistManager.deletePlaylist(name);

}

void MediaPlaylistManagerController::displayAllPlaylist()
{
    playlistManagerView->displayAllPlaylist(playlistManager);
}

void MediaPlaylistManagerController::handlerPlaylistManager()
{
    int choice;
    while (true) {
        playlistManagerView->showMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            createPlaylist(); 
            break;
        case 2:
        {
            deletePlaylist();
            break;
        }
        case 3:
            displayAllPlaylist(); 
            break;
        case 0:
            return; 
        default:
            std::cerr << "Invalid choice. Please try again.\n";
            break;
        }
    }
}