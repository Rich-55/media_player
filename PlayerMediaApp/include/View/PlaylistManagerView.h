#ifndef PLAYLIST_MANAGER_VIEW_H
#define PLAYLIST_MANAGER_VIEW_H

#include "BaseView.h"

class PlaylistManagerView : public BaseView {
    private:
    public:
        PlaylistManagerView();
        
        int showMenu() override;

        void displayAllPlaylist(PlaylistManager) override;
};


#endif
