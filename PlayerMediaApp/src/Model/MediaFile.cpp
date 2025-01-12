#include "../../include/Model/MediaFile.h"
#include <iostream>
MediaFile::MediaFile(){}
MediaFile::MediaFile(const std::string& fileName, const std::string& pathName, double size, const std::string& duration, const std::string& fileType)
    : fileName(fileName), pathName(pathName), size(size), duration(duration), fileType(fileType) {}

MediaFile::~MediaFile() {}

std::string MediaFile::getName() const { return this->fileName;}
std::string MediaFile::getPath() const { return this->pathName;}
double MediaFile::getSize() const { return this->size;}
std::string MediaFile::getDuration() const { return this->duration;}

void MediaFile::setType(std::string type){
    this->fileType = type;
}


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
    this->size = audioProps->length();
    
    this->duration = std::to_string(audioProps->length() / 60) + "m " +
                       std::to_string(audioProps->length() % 60) + "s";
}


void MediaFile::detailMediaFile() const {
    std::cout << "File Name: " << fileName << "\nPath: " << pathName
              << "\nSize: " << size << " MB\nDuration: " << duration
              << "\nFile Type: " << fileType << std::endl;
}

void MediaFile::editMediaFile() {
    std::cout << "Editing media file: " << fileName << std::endl;
}

std::string MediaFile::getType() {
    return "";
}

std::string MediaFile::getCodec() {
    return "";
}

int MediaFile::getBitrate() {
    return 0;
}

std::string MediaFile::getResolution() {
    return "";
}

std::string MediaFile::getTitle() const {
    return "";
}

std::string MediaFile::getAlbum() const {
    return "";
}

std::string MediaFile::getArtist() const {
    return "";
}

int MediaFile::getYear() const {
    return 0;
}


 // Tạo đối tượng TagLib::MPEG::File từ đường dẫn tệp

// Phương thức chỉnh sửa metadata
// void MediaFile::editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue) {

// }