#include "../../include/Model/VideoFile.h"

// Constructor
VideoFile::VideoFile(std::string fileName, std::string pathName, double size, std::string duration, std::string fileType,
                     std::string codec, int bitrate, std::string resolution)
    : MediaFile(fileName, pathName, size, duration, fileType), codec(codec), bitrate(bitrate), resolution(resolution) {}

// Getters
std::string VideoFile::getCodec() const { return codec; }
int VideoFile::getBitrate() const { return bitrate; }
std::string VideoFile::getResolution() const { return resolution; }

// Overridden methods
void VideoFile::detailMediaFile() const {
    MediaFile::detailMediaFile();
    std::cout << "Codec: " << codec << "\nBitrate: " << bitrate << " kbps"
              << "\nResolution: " << resolution << std::endl;
}

void VideoFile::editMediaFile() {
    std::cout << "Editing video file: " << fileName << std::endl;
}

std::string VideoFile::getType() const {
    return "Video File";
}
