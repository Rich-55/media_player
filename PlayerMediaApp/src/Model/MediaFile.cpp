#include "../../include/Model/MediaFile.h"
#include <iostream>

MediaFile::MediaFile(std::string fileName, std::string pathName, double size, std::string duration, std::string fileType)
    : fileName(fileName), pathName(pathName), size(size), duration(duration), fileType(fileType) {}

MediaFile::~MediaFile() {}

std::string MediaFile::getName() const { return fileName; }
std::string MediaFile::getPath() const { return pathName; }
int MediaFile::getDuration() const { return std::stoi(duration); }
std::string MediaFile::getType() const { return fileType; }

void MediaFile::detailMediaFile() const {
    std::cout << "File Name: " << fileName << "\nPath: " << pathName
              << "\nSize: " << size << " MB\nDuration: " << duration
              << "\nFile Type: " << fileType << std::endl;
}

void MediaFile::editMediaFile() {
    std::cout << "Editing media file: " << fileName << std::endl;
}
// Phương thức chỉnh sửa metadata
void MediaFile::editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue) {
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);
    if (tag) {
        TagLib::ByteVector keyByteVector(key.c_str());
        auto frameList = tag->frameListMap()[keyByteVector];
        if (!frameList.isEmpty()) {
            auto *frame = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frameList.front());
            if (frame) {
                frame->setText(TagLib::String(newValue, TagLib::String::UTF8));
                std::cout << "Updated key: " << key << " with new value: " << newValue << std::endl;
            }
        } else {
            std::cout << "Key not found: " << key << std::endl;
        }
    }
}