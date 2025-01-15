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

        std::vector<std::shared_ptr<MediaFile>> getListMediaFiles();

        void addMediaFile(std::shared_ptr<MediaFile>);

        bool deleteMediaFile(std::string& fileName);

        bool isEmpty() const; // Phương thức kiểm tra playlist có rỗng không


        ~Playlist();
};

#endif
