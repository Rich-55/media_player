#ifndef PLAYLIST_MANAGER_H
#define PLAYLIST_MANAGER_H

#include "Playlist.h"

class PlaylistManager{
    private:
        std::vector<std::shared_ptr<Playlist>> listPlaylist;
    public:
        PlaylistManager();
        virtual ~PlaylistManager();
        
        virtual bool checkPlaylist();
        virtual bool checkPlaylistName(std::string playlistName);

        virtual std::shared_ptr<Playlist> getPlaylist(std::string playlistName);

        virtual void loadPlaylist(std::shared_ptr<Playlist> playlist);
        virtual void addPlaylist(std::shared_ptr<Playlist> playlist);
        virtual bool deletePlaylist(std::string playlistName);

        virtual std::vector<std::shared_ptr<Playlist> > getAllPlaylist();

        

};

#endif
