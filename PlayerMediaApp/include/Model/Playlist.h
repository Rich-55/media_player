#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "MediaFile.h"

class Playlist {
    private:
        std::string playlistName;
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
    public:
        Playlist(std::string playlistName);
        virtual ~Playlist();
        
        std::string getPlaylistName() const;
        virtual void setPlaylistName(std::string playlistName);

        virtual bool checkMediaFile(std::string fileName);

        std::vector<std::shared_ptr<MediaFile>> getListMediaFiles();
        virtual std::vector<std::string> getListPathMediaFiles();

        void loadMediaFile(std::shared_ptr<MediaFile> mediafile);
        virtual void addMediaFile(std::shared_ptr<MediaFile> mediafile);

        virtual void deleteMediaFile(std::string fileName);

        
};

#endif
