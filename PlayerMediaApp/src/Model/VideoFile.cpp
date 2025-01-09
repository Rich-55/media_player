#include "../../include/Model/VideoFile.h"

// Constructor
VideoFile::VideoFile(const std::string& fileName, const std::string& pathName, double size, const std::string& duration, const std::string& fileType,
    const std::string& codec, int bitrate, const std::string& resolution)
        : MediaFile(fileName, pathName, size, duration, fileType), codec(codec), bitrate(bitrate), resolution(resolution) {
        TagLib::FileRef file(pathName.c_str());
        TagLib::AudioProperties *audioProps = file.audioProperties();
        if(!audioProps){
            std::cerr << "File not found!";
        }
        this->codec = "h264";
        this->bitrate = audioProps->bitrate();
        this->resolution = "1280x720";

        size_t lastSlash = pathName.find_last_of("/\\"); 
        size_t lastDot = pathName.find_last_of('.');   
        setName(pathName.substr(lastSlash + 1, lastDot - lastSlash - 1));

        setSize(audioProps->length());

        setDuration(std::to_string(audioProps->length() / 60) + "m " +
                       std::to_string(audioProps->length() % 60) + "s");
        setFileType("Video");
}   

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
    std::cout << "Editing video file: " << getName() << std::endl;
    TagLib::MPEG::File tagFile(getPath().c_str());
    
}

std::string VideoFile::getType() {
    return "Video File";
}
