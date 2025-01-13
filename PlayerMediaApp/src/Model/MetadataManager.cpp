#include "../../include/Model/MetadataManager.h"
MetadataManager::MetadataManager(){}

void MetadataManager::addMediaFile( std::string pathName, std::string type)
{   
    if(type == "Video"){
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
    }else {
        // std::cout << "adding audio" << std::endl;
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<AudioFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
    }
}

void MetadataManager::getData(){
    std::shared_ptr<MediaFile> file = listMediaFiles[0];
    std::cout << file->getName();
}


std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllMediaFile() {
    return this->listMediaFiles;
}

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllAudioFiles() {
    std::vector<std::shared_ptr<MediaFile>> listAudioFiles;
    for (const auto& file : listMediaFiles) {
        if (file->getType() == "Audio") {
            listAudioFiles.push_back(file);
        }
    }
    return listAudioFiles;
}

void MetadataManager::editMediaFile(std::string fileName) {
    auto it = std::find_if(listMediaFiles.begin(), listMediaFiles.end(),
        [&](const std::shared_ptr<MediaFile>& f) {
            return f->getName() == fileName;
        });

    if (it != listMediaFiles.end()) {
        // set logic here
        // (*it)->getName() = fileName; 
        // (*it)->getPath() = file->getPath();
        std::cout << "Edited media file: " << (*it)->getName() << std::endl;
    } else {
        std::cout << "Media file not found for editing." << std::endl;
    }
}

void MetadataManager::removeMediaFile(std::string fileName) {
    auto it = std::find_if(listMediaFiles.begin(), listMediaFiles.end(),
        [&](const std::shared_ptr<MediaFile>& f) {
            return f->getName() == fileName;
        });

    if (it!= listMediaFiles.end()) {
        listMediaFiles.erase(it);
        std::cout << "Removed media file: " << fileName << std::endl;
    } else {
        std::cout << "Media file not found for removal." << std::endl;
    }
}
