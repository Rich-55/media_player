#ifndef MEDIA_FILE_MANAGER_H
#define MEDIA_FILE_MANAGER_H

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
        virtual ~MediaFileManager();

        virtual std::shared_ptr<MediaFile> getMediaFile(std::string fileName);
        virtual std::shared_ptr<MediaFile> getMediaFileByPath(std::string pathName);

        bool checkFileExists(std::string fileName);

        virtual bool addMediaFile(std::string pathName, std::string type);

        virtual bool loadMediaFile(std::string pathName, std::string type);

        virtual bool deleteMediaFile(std::string fileName);

        void updateDatabase();

        virtual std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();

        virtual std::vector<std::shared_ptr<MediaFile>> getAllAudioFiles();

        virtual std::vector<std::shared_ptr<MediaFile>> getAllVideoFiles();
    
};

#endif


