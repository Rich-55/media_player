#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "MediaFile.h"

class Playlist {
    private:
        std::string playlistName;
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
    public:
        Playlist(std::string playlistName);

        std::string getPlaylistName() const;
        void setPlaylistName(std::string playlistName);

        bool checkMediaFile(std::string fileName);

        std::vector<std::shared_ptr<MediaFile>> getListMediaFiles();
        std::vector<std::string> getListPathMediaFiles();

        void loadMediaFile(std::shared_ptr<MediaFile> mediafile);
        void addMediaFile(std::shared_ptr<MediaFile> mediafile);

        void deleteMediaFile(std::string fileName);

        ~Playlist();
};

#endif
