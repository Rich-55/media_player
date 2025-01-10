#include "../../include/Model/VideoFile.h"

VideoFile::VideoFile(){}
// Constructor
VideoFile::VideoFile(const std::string& fileName, const std::string& pathName, double size, const std::string& duration, const std::string& fileType,
    const std::string& codec, int bitrate, const std::string& resolution)
        : MediaFile(fileName, pathName, size, duration, fileType), codec(codec), bitrate(bitrate), resolution(resolution) {}   

std::string VideoFile::getCodec() { return this->codec;}
int VideoFile::getBitrate() { return this->bitrate;}
std::string VideoFile::getResolution() { return this->resolution;}

void VideoFile::inputMediaFile(std::string pathName){
    MediaFile::inputMediaFile(pathName);
    TagLib::FileRef file(pathName.c_str());
    TagLib::AudioProperties *audioProps = file.audioProperties();
        if(!audioProps){
            std::cerr << "File not found!";
        }
    this->codec = "h264";
    this->bitrate = audioProps->bitrate();
    this->resolution = "1280x720";
    setType("Video File");
   
}

// Overridden methods
void VideoFile::detailMediaFile() const {
    MediaFile::detailMediaFile();
    std::cout << "Codec: " << codec << "\nBitrate: " << bitrate << " kbps"
              << "\nResolution: " << resolution << std::endl;
}

void VideoFile::editMediaFile() {
    std::cout << "Editing video file: " << getName() << std::endl;
    TagLib::MPEG::File tagFile(getPath().c_str());
    
}

std::string VideoFile::getType() {
    return "Video File";
}
