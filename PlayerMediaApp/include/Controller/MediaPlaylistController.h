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
        MediaPlaylistController(MediaFileManager& mediaFileManager, FolderManager& folderManager, std::shared_ptr<Playlist> playlistModel, std::shared_ptr<BaseView> mediaManagerView, std::shared_ptr<BaseView> playlistHandlerView);
        
        void handlerPlaylist();

        void setNamePlaylist(std::string name);

        std::vector<std::string> getListPathMediaFiles();

        bool addMediaFileInPlaylist(std::string fileName);

        bool addMediaFileByFolder();

        bool deleteMediaFileInPlaylist(std::string fileName);

        void displayAllMediaFilesInPlaylist();

        int showMenuWithMediaListInPlaylist();

};

#endif
