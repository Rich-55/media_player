#ifndef MEDIA_PLAYLIST_CONTROLLER_H
#define MEDIA_PLAYLIST_CONTROLLER_H


#include "../Model/Playlist.h"
#include "../Model/MediaFileManager.h"
#include "../View/PlaylistHandlerView.h"  

#include "../utils/PlaylistHandlerException.h"
#include "../utils/ConfigLoader.h"


class MediaPlaylistController{
    private:
        MediaFileManager& mediaFileManager;
        std::shared_ptr<Playlist> playlist;
        std::shared_ptr<BaseView> playlistHandlerView;
    public:
        MediaPlaylistController(MediaFileManager& , std::shared_ptr<Playlist>, std::shared_ptr<BaseView>);
        
        void handlerPlaylist();

        void addMediaFile(std::shared_ptr<MediaFile>);

        void deleteMediaFile();

        void displayAllMediaFiles();

};

#endif
