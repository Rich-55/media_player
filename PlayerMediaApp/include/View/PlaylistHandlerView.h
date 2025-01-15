#ifndef PLAYLIST_HANDLER_VIEW_H
#define PLAYLIST_HANDLER_VIEW_H

#include "ViewBase.h"
#define Add_MediaFile 1
#define Delete_MediaFile 2
#define Display_All_MediaFiles 3
#define exit 0

class PlaylistHandlerView : public ViewBase {
    private:
    public:
        PlaylistHandlerView();
        
        void showMenu()override;

        void displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>) override;
        
};


#endif
