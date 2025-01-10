#include "../../include/Model/MetadataManager.h"
MetadataManager::MetadataManager(){}

void MetadataManager::addMediaFile( std::string pathName, std::string type)
{   
    if(type == "Video"){
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName);
        listMedias.push_back(mediaFile);
    }else{

    }
}

// void MetadataManager::editMediaFile(std::shared_ptr<MediaFile> mediaFile)
// {
    
// }

// void MetadataManager::deleteMediaFile(std::shared_ptr<MediaFile>)
// {

// }

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllMediaFile(){
    return this->listMedias;
}
