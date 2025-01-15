#ifndef PLAYLIST_MANAGER_VIEW_H
#define PLAYLIST_MANAGER_VIEW_H

#include "ViewBase.h"
#define Create_new_playlist 1
#define Delete_playlist_from_mediafile 2
#define Show_all_playlist 3
#define Exit 0

class PlaylistManagerView : public ViewBase {
    private:
    public:
        PlaylistManagerView();
        
        void showMenu() override;

        void displayAllPlaylist(PlaylistManager) override;
};


#endif
