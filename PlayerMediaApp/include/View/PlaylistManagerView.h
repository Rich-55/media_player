#ifndef PLAYLIST_MANAGER_VIEW_H
#define PLAYLIST_MANAGER_VIEW_H

#include "BaseView.h"

class PlaylistManagerView : public BaseView {
    private:
    public:
        PlaylistManagerView();
        
        int showMenu() override;

        int showMenuWithPlaylist(std::vector<std::shared_ptr<Playlist>>) override;
        bool showConfirmMenu(std::string) override;
        void showNotificationMessage(std::string, std::string) override;
        std::string showMenuCreatePlaylist() override;

        std::string displayAllPlaylist(PlaylistManager) override;
};


#endif
