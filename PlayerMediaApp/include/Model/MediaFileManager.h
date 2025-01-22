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

        std::shared_ptr<MediaFile> getMediaFile(std::string);
        std::shared_ptr<MediaFile> getMediaFileByPath(std::string);

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


