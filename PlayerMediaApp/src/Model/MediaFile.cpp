#include "../../include/Model/MediaFile.h"
#include <iostream>

MediaFile::MediaFile(const std::string& fileName, const std::string& pathName, double size, const std::string& duration, const std::string& fileType)
    : fileName(fileName), pathName(pathName), size(size), duration(duration), fileType(fileType) {}

MediaFile::~MediaFile() {}

std::string MediaFile::getName() const { return this->fileName;}
std::string MediaFile::getPath() const { return this->pathName;}
double MediaFile::getSize() const { return this->size;}
std::string MediaFile::getDuration() const { return this->duration;}


void MediaFile::setName(std::string fileName){this->fileName = fileName;}
void MediaFile::setPath(std::string pathName){this->pathName = pathName;}
void MediaFile::setSize(double size){this->size = size;}
void MediaFile::setDuration(std::string duration){this->duration = duration;}
void MediaFile::setFileType(std::string fileType){this->fileType = fileType;}

void MediaFile::detailMediaFile() const {
    std::cout << "File Name: " << fileName << "\nPath: " << pathName
              << "\nSize: " << size << " MB\nDuration: " << duration
              << "\nFile Type: " << fileType << std::endl;
}

void MediaFile::editMediaFile() {
    std::cout << "Editing media file: " << fileName << std::endl;
}
// Phương thức chỉnh sửa metadata
// void MediaFile::editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue) {

// }