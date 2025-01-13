#include "../../include/Model/MetadataManager.h"
MetadataManager::MetadataManager(){}

std::shared_ptr<MediaFile> MetadataManager::getMediaFile(std::string fileName)
{
   for(std::shared_ptr<MediaFile> file : listMediaFiles){
        if(file->getName() == fileName){
            return file;
        }
    } 
    return nullptr;
}

std::unordered_set<std::string> MetadataManager::getListFileAdded()
{
    return listFileAdded;
}
void MetadataManager::clearListFileAdded()
{
    this->listFileAdded.clear();
}

#include <fstream>

void writePathToFile(const std::string &pathName, const std::string &filePath = "database/video/video.data") {
    std::ofstream file(filePath, std::ios::app); 
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << '\n';
        return;
    }
    file << pathName << '\n'; 
    file.close();
}


void MetadataManager::addMediaFile( std::string pathName, std::string type)
{   
    std::string fileName = pathName.substr(pathName.find_last_of("/\\") + 1);
    TagLib::MP4::File fileTemp(pathName.c_str());
    unsigned long long size = fileTemp.length();

    for(auto file : listMediaFilesSize){
        if(file.first == fileName){
            if(file.second == size){
                std::cerr << "File already exists!\n";
                return;
            }
        }
    }

    listMediaFilesSize[fileName] = size;

    if(type == "Video"){        
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
        writePathToFile(pathName, "database/video/video.data");
        std::cout << "Video \""<< mediaFile->getName() << "\" add successfully!" << std::endl;
    }else {
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<AudioFile>();
        mediaFile->inputMediaFile(pathName);
        listMediaFiles.push_back(mediaFile);
        writePathToFile(pathName, "database/audio/audio.data");
        std::cout << "Audio \""<< mediaFile->getName() << "\" add successfully!" << std::endl;
    }
    listFileAdded.insert(fileName);
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

void MetadataManager::updateDatabase() 
{
    std::ofstream videoFile("database/video/video.data", std::ios::trunc);
    if (!videoFile.is_open()) {
        std::cerr << "Error: Unable to open video database file.\n";
        return;
    }

    std::ofstream audioFile("database/audio/audio.data", std::ios::trunc);
    if (!audioFile.is_open()) {
        std::cerr << "Error: Unable to open audio database file.\n";
        return;
    }

    for (const auto &mediaFile : listMediaFiles) {
        if (mediaFile->getType() == "Video") {
            videoFile << mediaFile->getPath() << '\n';
        } else if (mediaFile->getType() == "Audio") {
            audioFile << mediaFile->getPath() << '\n';
        }
    }

    videoFile.close();
    audioFile.close();

    std::cout << "Databases updated successfully!" << std::endl;
}


void MetadataManager::deleteMediaFile(std::string fileName) {
    for (auto it = listMediaFiles.begin(); it != listMediaFiles.end(); ++it) {
        if ((*it)->getName() == fileName) {

            listMediaFiles.erase(it);

            updateDatabase();

            std::cout << "File \"" << fileName << "\" deleted successfully!" << std::endl;
            return;
        }
    }
    std::cerr << "Error: File not found.\n";
}
