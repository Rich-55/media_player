#ifndef MEDIA_PLAYLIST_MANAGER_CONTROLLER_H
#define MEDIA_PLAYLIST_MANAGER_CONTROLLER_H



#include "../Model/PlaylistManager.h"
#include "../Model/MediaFileManager.h"
#include "../Model/FolderManager.h"
#include "../View/PlaylistManagerView.h"
#include "../View/PlaylistHandlerView.h"
#include "../View/BaseView.h"
#include "MediaPlaylistController.h"

#include "../utils/PlaylistManagerException.h"
#include "../utils/ConfigLoader.h"


class MediaPlaylistManagerController{
    private:
        PlaylistManager& playlistManager;
        MediaFileManager& mediaFileManager;
        FolderManager& folderManager;
        std::shared_ptr<BaseView> mediaManagerView;
        std::shared_ptr<BaseView> playlistManagerView;
        std::shared_ptr<BaseView> playlistHandlerView;
        std::unordered_map<std::string, std::shared_ptr<MediaPlaylistController>> listMediaPlaylistController;
    public:
        MediaPlaylistManagerController(PlaylistManager&, MediaFileManager&, FolderManager&, std::shared_ptr<BaseView>, std::shared_ptr<BaseView>, std::shared_ptr<BaseView>);

        void handlerPlaylistManager();

        void addMediaPlaylistController(std::string, std::shared_ptr<MediaPlaylistController>);
        std::shared_ptr<MediaPlaylistController> getMediaPlaylistController(const std::string& playlistName);

        std::string createPlaylist(std::string);
        
        bool deletePlaylist(std::string);

        std::string displayAllPlaylist();


};

#endif
