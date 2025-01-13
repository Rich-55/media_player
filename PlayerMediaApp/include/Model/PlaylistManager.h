#ifndef PLAYLIST_MANAGER_H
#define PLAYLIST_MANAGER_H

#include "Playlist.h"

class PlaylistManager{
    private:
        std::vector<std::shared_ptr<Playlist>> listPlaylist;
    public:
        PlaylistManager();

        bool checkPlaylist();

        bool checkPlaylistName(std::string);

        std::shared_ptr<Playlist> getPlaylist(std::string);

        void addPlaylist(std::shared_ptr<Playlist> );

        void deletePlaylist(std::string);

        std::vector<std::shared_ptr<Playlist> > getAllPlaylist();

        ~PlaylistManager();

};

#endif
