#ifndef MEDIA_PLAYLIST_CONTROLLER_H
#define MEDIA_PLAYLIST_CONTROLLER_H


#include "../Model/Playlist.h"
#include "../Model/MediaFileManager.h"
#include "../Model/FolderManager.h"
#include "../View/PlaylistHandlerView.h"  

#include "../utils/PlaylistHandlerException.h"
#include "../utils/ConfigLoader.h"


class MediaPlaylistController{
    private:
        MediaFileManager& mediaFileManager;
        FolderManager& folderManager;
        std::shared_ptr<Playlist> playlist;
        
        std::shared_ptr<BaseView> mediaManagerView;
        std::shared_ptr<BaseView> playlistHandlerView;
    public:
        MediaPlaylistController(MediaFileManager&, FolderManager& , std::shared_ptr<Playlist>, std::shared_ptr<BaseView>, std::shared_ptr<BaseView>);
        
        void handlerPlaylist();

        void setNamePlaylist(std::string);

        std::vector<std::string> getListPathMediaFiles();

        bool addMediaFileInPlaylist(std::string);

        bool addMediaFileByFolder();

        bool deleteMediaFileInPlaylist(std::string );

        void displayAllMediaFilesInPlaylist();

        int showMenuWithMediaListInPlaylist();

};

#endif
