#include "../../include/Controller/MediaPlaylistController.h"

MediaPlaylistController::MediaPlaylistController(std::shared_ptr<Playlist> p, std::shared_ptr<ViewBase> v) : playlist(p), playlistHandlerView(v){}

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