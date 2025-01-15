#ifndef MEDIA_PLAYLIST_MANAGER_CONTROLLER_H
#define MEDIA_PLAYLIST_MANAGER_CONTROLLER_H



#include "../Model/PlaylistManager.h"
#include "../View/PlaylistManagerView.h"

#include "../utils/PlaylistManagerException.h"
#include "../utils/ConfigLoader.h"


class MediaPlaylistManagerController{
    private:
        PlaylistManager& playlistManager;
        std::shared_ptr<BaseView> playlistManagerView;
    public:
        MediaPlaylistManagerController(PlaylistManager&, std::shared_ptr<BaseView>);

        void handlerPlaylistManager();

        void createPlaylist();
        
        void deletePlaylist();

        void displayAllPlaylist();


};

#endif
