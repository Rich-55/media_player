#ifndef PLAYLIST_H
#define PLAYLIST_H

#include<iostream>
#include<string>
#include<memory>
#include<vector>

#include "../Model/MediaFile.h"

class Playlist {
    private:
        std::string name;
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
    public:
        Playlist(std::string);

        std::string getName() const;

        bool checkMediaFile(std::string);

        std::vector<std::shared_ptr<MediaFile>> getListMediaFiles();

        void addMediaFile(std::shared_ptr<MediaFile>);

        void deleteMediaFile(std::string);

        ~Playlist();
};

#endif
