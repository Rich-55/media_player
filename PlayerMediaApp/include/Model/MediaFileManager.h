#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <fstream>
#include "../Model/MediaFile.h"
#include "../Model/VideoFile.h"
#include "../Model/AudioFile.h"
class MediaFileManager{
    private:
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
        std::unordered_map<std::string, unsigned long long> listMediaFilesSize;
        std::unordered_set<std::string> listFileAdded;
    public:
        MediaFileManager();

        std::shared_ptr<MediaFile> getMediaFile(std::string);

        bool checkFileExists(std::string);

        bool addMediaFile(std::string, std::string);

        bool loadMediaFile(std::string, std::string);

        bool deleteMediaFile(std::string);


        void updateDatabase();

        std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();

        std::vector<std::shared_ptr<MediaFile>> getAllAudioFiles();

        std::vector<std::shared_ptr<MediaFile>> getAllVideoFiles();
    
};

#endif


