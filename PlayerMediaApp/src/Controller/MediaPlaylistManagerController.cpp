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