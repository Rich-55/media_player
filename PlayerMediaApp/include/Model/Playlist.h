#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "MediaFile.h"

class Playlist {
    private:
        std::string name;
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
    public:
        Playlist(std::string name);

        std::string getName() const;
        void setName(std::string name);

        bool checkMediaFile(std::string file);

        std::vector<std::shared_ptr<MediaFile>> getListMediaFiles();
        std::vector<std::string> getListPathMediaFiles();

        void loadMediaFile(std::shared_ptr<MediaFile> file);
        void addMediaFile(std::shared_ptr<MediaFile> file);

        void deleteMediaFile(std::string fileName);

        ~Playlist();
};

#endif
