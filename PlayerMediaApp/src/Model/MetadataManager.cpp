#include "../../include/Model/MetadataManager.h"
MetadataManager::MetadataManager(){}

void MetadataManager::addMediaFile( std::string pathName, std::string type)
{   
    if(type == "Video"){
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
    }else {
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<AudioFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
    }
}

// void MetadataManager::editMediaFile(std::shared_ptr<MediaFile> mediaFile)
// {
    
// }

// void MetadataManager::deleteMediaFile(std::shared_ptr<MediaFile>)
// {

// }

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllMediaFile()
{
    return this->listMediaFiles;
}

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllAudioFiles()
{
    std::vector<std::shared_ptr<MediaFile>> audioFiles;
    for(auto file : listMediaFiles){
        if(file->getType() == "Audio"){
            audioFiles.push_back(file);
        }
    }
    return audioFiles;
}

// void MetadataManager::addMediaFileFromUSB(std::shared_ptr<MediaFile> file) {
//     listMediaFiles.push_back(file);
//     std::cout << "Added media file from USB: " << file->getName() << std::endl;
// }

// void addMediaFileFromPC(std::shared_ptr<MediaFile> file) {
//         listMediaFiles.push_back(file);
//         std::cout << "Added media file from PC: " << file->getName() << std::endl;
// }

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

// void MetadataManager::removeMediaFile(std::shared_ptr<MediaFile> file) {
//     auto it = std::remove_if(listMediaFiles.begin(), listMediaFiles.end(),
//         [&](const std::shared_ptr<MediaFile>& f) {
//             return f->getName() == file->getName();
//         });

//     if (it != listMediaFiles.end()) {
//         listMediaFiles.erase(it, listMediaFiles.end());
//         std::cout << "Removed media file: " << file->getName() << std::endl;
//     } else {
//         std::cout << "Media file not found for removal." << std::endl;
//     }
// }

// void MetadataManager::getAllMediaFiles() const {
//     std::cout << "--- All Media Files ---" << std::endl;
//     if (listMediaFiles.empty()) {
//         std::cout << "No media files found." << std::endl;
//     } else {
//         for (const auto& file : listMediaFiles) {
//             file.getName();
//             std::cout << "------------------------" << std::endl;
//         }
//     }
// }
