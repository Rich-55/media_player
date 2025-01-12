#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include "../Model/MediaFile.h"
#include "../Model/VideoFile.h"
#include "../Model/AudioFile.h"
class MetadataManager{
    private:
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
        std::unordered_set<std::string> listPaths;
    public:
        MetadataManager();

        std::shared_ptr<MediaFile> getMediaFile(std::string);

        void addMediaFile(std::string, std::string);

        void deleteMediaFile(std::string);

        std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();

        std::vector<std::shared_ptr<MediaFile>> getAllAudioFiles();

        std::vector<std::shared_ptr<MediaFile>> getAllVideoFiles();
    
};

#endif


