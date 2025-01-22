#ifndef PLAYLIST_MANAGER_VIEW_H
#define PLAYLIST_MANAGER_VIEW_H

#include "BaseView.h"

class PlaylistManagerView : public BaseView {
    private:
    public:
        PlaylistManagerView();
        
        int showMenu() override;

        int showMenuWithPlaylist(std::vector<std::shared_ptr<Playlist>> listPlaylist) override;
        bool showConfirmMenu(std::string message) override;
        void showNotificationMessage(std::string message, std::string type) override;
        std::string showMenuCreatePlaylist() override;

        std::string displayAllPlaylist(PlaylistManager playlistManager) override;
};


#endif
