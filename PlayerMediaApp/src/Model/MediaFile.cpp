#include "../../include/Model/MediaFile.h"

MediaFile::MediaFile(){}
MediaFile::MediaFile(const std::string& fileName, const std::string& pathName, unsigned long size, const std::string& duration, const std::string& fileType)
    : fileName(fileName), pathName(pathName), size(size), duration(duration), fileType(fileType) {}

MediaFile::~MediaFile() {}

std::string MediaFile::getName() const { return this->fileName;}
std::string MediaFile::getPath() const { return this->pathName;}
unsigned long MediaFile::getSize() const { return this->size;}
std::string MediaFile::getDuration() const { return this->duration;}

void MediaFile::setType(std::string type) { this->fileType = type;}

void MediaFile::inputMediaFile(std::string pathName){
    TagLib::FileRef file(pathName.c_str());
    TagLib::AudioProperties *audioProps = file.audioProperties();
    if(!audioProps){
        std::cerr << "File not found!";
    }
    size_t lastSlash = pathName.find_last_of("/\\"); 
    size_t lastDot = pathName.find_last_of('.');   
    this->fileName =  pathName.substr(lastSlash + 1, lastDot - lastSlash - 1);
    this->pathName = pathName;

    TagLib::MP4::File fileTemp(pathName.c_str());
    this->size = fileTemp.length();
    
    
    this->duration = std::to_string(audioProps->length() / 60) + "m " +
                       std::to_string(audioProps->length() % 60) + "s";
}

void MediaFile::addNewKey(const std::string&, const std::string&){}

void MediaFile::editKey(const std::string&, const std::string&) {}

void MediaFile::deleteKey(const std::string& ) {}

std::string MediaFile::getType() { return "";}

std::string MediaFile::getMetadata(const std::string&) const { return "";}

std::unordered_map<std::string, std::string> MediaFile::getAllMetadata() const { return {};}


