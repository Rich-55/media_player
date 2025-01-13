#ifndef PLAYLIST_MANAGER_VIEW_H
#define PLAYLIST_MANAGER_VIEW_H

#include "ViewBase.h"

class PlaylistManagerView : public ViewBase {
    private:
    public:
        PlaylistManagerView();
        
        void showMenu() override;

        void displayAllPlaylist(PlaylistManager) override;
};


#endif
