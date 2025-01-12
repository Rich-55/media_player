#include "../../include/Model/MetadataManager.h"
#include "../../include/Model/VideoFile.h" // Bổ sung
#include <iostream>

MetadataManager::MetadataManager() {}

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllMediaFile(){return this->listMediaFiles;}

void MetadataManager::getData(){
    std::shared_ptr<MediaFile> file = listMediaFiles[0];
    std::cout << file->getName();
}


// MetadataManager class implementation (Model)
void MetadataManager::addMediaFile(std::string pathName, std::string type) {

    if(type == "Video"){
        std::cout << "adding data" << std::endl;
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
    }
}

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllVideoFiles()

{
    std::vector<std::shared_ptr<MediaFile>> videoFiles;
    for(auto file : listMediaFiles){
        if(file->getType() == "video"){
            videoFiles.push_back(file);
        }
    }
    return videoFiles;
}

void MetadataManager::removeMediaFile(std::shared_ptr<MediaFile> file) {
    auto it = std::remove_if(listMediaFiles.begin(), listMediaFiles.end(),
        [&file](const std::shared_ptr<MediaFile>& media) {
            return media->getName() == file->getName();
        });
    if (it != listMediaFiles.end()) {
        listMediaFiles.erase(it, listMediaFiles.end());
        std::cout << "Removed media file: " << file->getName() << std::endl;
    } else {
        std::cout << "Media file not found: " << file->getName() << std::endl;
    }
}

void MetadataManager::editMediaFile(std::shared_ptr<MediaFile> file) {
    auto it = std::find_if(listMediaFiles.begin(), listMediaFiles.end(),
        [&](const std::shared_ptr<MediaFile>& f) {
            return f->getName() == file->getName();
        });

    if (it != listMediaFiles.end()) {
        (*it)->getName() = file->getName(); 
        (*it)->getPath() = file->getPath();
        std::cout << "Edited media file: " << (*it)->getName() << std::endl;
    } else {
        std::cout << "Media file not found for editing." << std::endl;
    }
}

MetadataManager::~MetadataManager() {
    // Hàm hủy (nếu không có gì đặc biệt, có thể để trống)
}