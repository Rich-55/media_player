#ifndef MEDIA_PLAYLIST_CONTROLLER_H
#define MEDIA_PLAYLIST_CONTROLLER_H

#include "../Model/Playlist.h"
#include "../View/PlaylistHandlerView.h"    
class MediaPlaylistController{
    private:
        std::shared_ptr<Playlist> playlist;
        std::shared_ptr<ViewBase> playlistHandlerView;
    public:
        MediaPlaylistController(std::shared_ptr<Playlist>, std::shared_ptr<ViewBase>);
        
        void addMediaFile(std::shared_ptr<MediaFile>);

        void deleteMediaFile();

        void displayAllMediaFiles();

};

#endif
