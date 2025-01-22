#ifndef PLAYLIST_HANDLER_VIEW_H
#define PLAYLIST_HANDLER_VIEW_H

#include "BaseView.h"

class PlaylistHandlerView : public BaseView {
    private:
    public:
        PlaylistHandlerView();
        
        int showMenu()override;

        bool showConfirmMenu(std::string message) override;

        void showNotificationMessage(std::string message, std::string type) override;

        std::string showMenuCreatePlaylist() override;

        int showMenuWithMediaListInPlaylist(std::shared_ptr<Playlist> playlist) override;

        std::string displayAllMediaFileInPlaylist(std::shared_ptr<Playlist> playlist) override;
        
        std::pair<std::string, std::string> displayAllFolder(std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>>
        listFolderDirectoryAndUSB) override;
};


#endif
