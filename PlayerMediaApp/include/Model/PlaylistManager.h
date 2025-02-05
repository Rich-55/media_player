#ifndef PLAYLIST_MANAGER_H
#define PLAYLIST_MANAGER_H

#include "Playlist.h"

class PlaylistManager{
    private:
        std::vector<std::shared_ptr<Playlist>> listPlaylist;
    public:
        PlaylistManager();

        bool checkPlaylist();
        bool checkPlaylistName(std::string playlistName);

        std::shared_ptr<Playlist> getPlaylist(std::string playlistName);

        virtual void loadPlaylist(std::shared_ptr<Playlist> playlist);
        void addPlaylist(std::shared_ptr<Playlist> playlist);
        bool deletePlaylist(std::string playlistName);

        std::vector<std::shared_ptr<Playlist> > getAllPlaylist();

        ~PlaylistManager();

};

#endif
