#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include "MediaFile.h"
#include "VideoFile.h"
#include "AudioFile.h"

class MediaFileManager{
    private:
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
        std::unordered_map<std::string, unsigned long long> listMediaFilesSize;
        std::unordered_set<std::string> listFileAdded;
    public:
        MediaFileManager();

        std::shared_ptr<MediaFile> getMediaFile(std::string fileName);
        std::shared_ptr<MediaFile> getMediaFileByPath(std::string pathName);

        bool checkFileExists(std::string fileName);

        bool addMediaFile(std::string pathName, std::string type);

        bool loadMediaFile(std::string pathName, std::string type);

        bool deleteMediaFile(std::string fileName);

        void updateDatabase();

        std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();

        std::vector<std::shared_ptr<MediaFile>> getAllAudioFiles();

        std::vector<std::shared_ptr<MediaFile>> getAllVideoFiles();
    
};

#endif


