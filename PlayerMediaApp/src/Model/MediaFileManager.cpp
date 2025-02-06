#include "../../include/Model/MediaFileManager.h"
MediaFileManager::MediaFileManager(){}

std::shared_ptr<MediaFile> MediaFileManager::getMediaFile(std::string fileName)
{
   for(std::shared_ptr<MediaFile> file : listMediaFiles){
        if(file->getFileName() == fileName){
            return file;
        }
    } 
    return nullptr;
}

std::shared_ptr<MediaFile> MediaFileManager::getMediaFileByPath(std::string pathName)
{
   for(std::shared_ptr<MediaFile> file : listMediaFiles){
        if(file->getPath() == pathName){
            return file;
        }
    } 
    return nullptr;
}

bool MediaFileManager::checkFileExists(std::string fileName)
{
    for(std::shared_ptr<MediaFile> file : listMediaFiles){
        if(file->getFileName() == fileName){
            return true;
        }
    } 
    return false;
}




void writePathToFile(const std::string &pathName, const std::string &filePath = "database/video/video.data") 
{
    std::ofstream file(filePath, std::ios::app); 
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << '\n';
        return;
    }
    file << pathName << '\n'; 
    file.close();
}

bool MediaFileManager::loadMediaFile(std::string pathName, std::string type)
{   
    bool result = false;
    bool isSame = false;
    std::string fileName = pathName.substr(pathName.find_last_of("/\\") + 1);
    TagLib::MP4::File fileTemp(pathName.c_str());
    unsigned long long size = fileTemp.length();

    for(auto file : listMediaFilesSize){
        if(file.first == fileName){
            if(file.second == size){
                std::cerr << "File already exists!\n";
                return result;
            }else{
                isSame = true;
            }
        }
    }

    listMediaFilesSize[fileName] = size;

    if(type == "Video"){        
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName, isSame);
        listMediaFiles.push_back(mediaFile);
        result = true;
    }else {
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<AudioFile>();
        mediaFile->inputMediaFile(pathName, isSame);
        listMediaFiles.push_back(mediaFile);
        result = true;
    }
    listFileAdded.insert(fileName);
    return result;
}

bool MediaFileManager::addMediaFile(std::string pathName, std::string type)
{   
    bool result = false;
    bool isSame = false;
    std::string fileName = pathName.substr(pathName.find_last_of("/\\") + 1);
    TagLib::MP4::File fileTemp(pathName.c_str());
    unsigned long long size = fileTemp.length();

    for(auto file : listMediaFilesSize){
        if(file.first == fileName){
            if(file.second == size){
                return result;
            }else{
                isSame = true;
            }
        }
    }

    listMediaFilesSize[fileName] = size;

    if(type == "Video"){        
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<VideoFile>();
        mediaFile->inputMediaFile(pathName, isSame);
        listMediaFiles.push_back(mediaFile);
        writePathToFile(pathName, "database/video/video.data");
        result = true;
    }else {
        std::shared_ptr<MediaFile> mediaFile =  std::make_shared<AudioFile>();
        mediaFile->inputMediaFile(pathName, isSame);
        listMediaFiles.push_back(mediaFile);
        writePathToFile(pathName, "database/audio/audio.data");
        result = true;
    }
    listFileAdded.insert(fileName);
    return result;
}

std::vector<std::shared_ptr<MediaFile>> MediaFileManager::getAllMediaFile()
{   
    
    return this->listMediaFiles;
}

std::vector<std::shared_ptr<MediaFile>> MediaFileManager::getAllAudioFiles()
{
    std::vector<std::shared_ptr<MediaFile>> audioFiles;
    for(auto file : listMediaFiles){
        if(file->getType() == "Audio"){
            audioFiles.push_back(file);
        }
    }
    return audioFiles;
}

std::vector<std::shared_ptr<MediaFile>> MediaFileManager::getAllVideoFiles()
{
    std::vector<std::shared_ptr<MediaFile>> videoFiles;
    for(auto file : listMediaFiles){
        if(file->getType() == "Video"){
            videoFiles.push_back(file);
        }
    }
    return videoFiles;
}

void MediaFileManager::updateDatabase() 
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


bool MediaFileManager::deleteMediaFile(std::string fileName) 
{
    for (auto it = listMediaFiles.begin(); it != listMediaFiles.end(); ++it) {
        if ((*it)->getFileName() == fileName) {

            listMediaFiles.erase(it);

            updateDatabase();
            
            std::cout << "File \"" << fileName << "\" deleted successfully!" << std::endl;
            
            return true;
        }
    }
    return false;
}
