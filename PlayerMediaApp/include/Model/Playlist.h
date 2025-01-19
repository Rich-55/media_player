#ifndef PLAYLIST_H
#define PLAYLIST_H

#include<iostream>
#include<string>
#include<memory>
#include<vector>
#include <fstream>
#include <filesystem>
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
        std::vector<std::string> getListPathMediaFiles();

        void loadMediaFile(std::shared_ptr<MediaFile>);
        void addMediaFile(std::shared_ptr<MediaFile>);

        void deleteMediaFile(std::string);

        ~Playlist();
};

#endif
