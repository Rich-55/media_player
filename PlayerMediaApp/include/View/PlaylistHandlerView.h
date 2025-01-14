#ifndef PLAYLIST_HANDLER_VIEW_H
#define PLAYLIST_HANDLER_VIEW_H

#include "BaseView.h"

class PlaylistHandlerView : public BaseView {
    private:
    public:
        PlaylistHandlerView();
        
        void showMenu()override;

        void displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>) override;
        
};


#endif
