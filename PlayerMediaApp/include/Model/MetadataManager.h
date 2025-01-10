#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "../Model/MediaFile.h"
#include "../Model/VideoFile.h"
class MetadataManager{
    private:
        std::vector<std::shared_ptr<MediaFile>> listMedias;
    public:
        MetadataManager();

        void addMediaFile(std::string, std::string);

        void editMediaFile(std::shared_ptr<MediaFile>);

        void deleteMediaFile(std::shared_ptr<MediaFile>);

        std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();
    
};

#endif
