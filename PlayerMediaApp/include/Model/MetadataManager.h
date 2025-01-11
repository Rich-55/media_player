#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "../Model/MediaFile.h"
#include "../Model/VideoFile.h"
#include "../Model/AudioFile.h"
class MetadataManager{
    private:
        std::vector<std::shared_ptr<MediaFile>> listMediaFiles;
    public:
        MetadataManager();

        void addMediaFile(std::string, std::string);

        void getData();

        void editMediaFile(std::shared_ptr<MediaFile>);

        void deleteMediaFile(std::shared_ptr<MediaFile>);

        std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();

        std::vector<std::shared_ptr<MediaFile>> getAllAudioFiles();
    
};

#endif


