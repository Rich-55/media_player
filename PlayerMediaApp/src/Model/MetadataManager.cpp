#include "../../include/Model/MetadataManager.h"
MetadataManager::MetadataManager(){}

std::shared_ptr<MediaFile> MetadataManager::getMediaFile(std::string fileName)
{
   for(std::shared_ptr<MediaFile> file : listMediaFiles){
        if(file->getName() == fileName){
            return file;
        }
    } 
    throw std::invalid_argument("File not found!");

}

void MetadataManager::addMediaFile( std::string pathName, std::string type)
{   
    if(listPaths.empty()){
        listPaths.insert(pathName);
    }else{
        if(listPaths.find(pathName) == listPaths.end()){
            listPaths.insert(pathName);
        }else{
            std::cerr << "File already exists!";
            return;
        }
    }

    if(type == "Video"){        
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
        std::cout << "Video \""<< mediaFile->getName() << "\" add successfully!" << std::endl;
    }else {
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<AudioFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
        std::cout << "Audio \""<< mediaFile->getName() << "\" add successfully!" << std::endl;
    }
}

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

std::vector<std::shared_ptr<MediaFile>> MetadataManager::getAllVideoFiles()
{
    std::vector<std::shared_ptr<MediaFile>> videoFiles;
    for(auto file : listMediaFiles){
        if(file->getType() == "Video"){
            videoFiles.push_back(file);
        }
    }
    return videoFiles;
}


void MetadataManager::deleteMediaFile(std::string fileName) {
    for (auto it = listMediaFiles.begin(); it != listMediaFiles.end(); ++it) {
        if ((*it)->getName() == fileName) {
            listMediaFiles.erase(it);
            std::cout << "File \"" << fileName << "\" deleted successfully!" << std::endl;
            return;
        }
    }
    std::cerr << "Error: File not found.\n";
}
