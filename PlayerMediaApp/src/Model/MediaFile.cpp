#include "../../include/Model/MediaFile.h"

MediaFile::MediaFile(){}
MediaFile::MediaFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& duration, const std::string& fileType)
    : fileName(fileName), pathName(pathName), size(size), duration(duration), fileType(fileType) {}

MediaFile::~MediaFile() {}

std::string MediaFile::getName() const { return this->fileName;}
std::string MediaFile::getPath() const { return this->pathName;}
unsigned long MediaFile::getSize() const { return this->size;}
std::string MediaFile::getDuration() const { return this->duration;}

void MediaFile::setType(std::string type) { this->fileType = type;}

void MediaFile::inputMediaFile(std::string pathName, bool isSame) {
    TagLib::FileRef file(pathName.c_str());
    TagLib::AudioProperties *audioProps = file.audioProperties();
    if (!audioProps) {
        std::cerr << "File not found!";
        return;
    }

    size_t lastSlash = pathName.find_last_of("/\\");
    this->fileName = pathName.substr(lastSlash + 1);
    
    if (isSame) {
        size_t dotPos = this->fileName.find_last_of('.');
        if (dotPos != std::string::npos) {
            this->fileName.insert(dotPos, "(another)");
        } else {
            this->fileName += "(another)";
        }
    }

    this->pathName = pathName;

    TagLib::MP4::File fileTemp(pathName.c_str());
    this->size = fileTemp.length();

    this->duration = std::to_string(audioProps->length() / 60) + "m " +
                     std::to_string(audioProps->length() % 60) + "s";
}


bool MediaFile::addNewKey(const std::string&, const std::string&){return false;}

bool MediaFile::editKey(const std::string&, const std::string&) {return false;}

bool MediaFile::deleteKey(const std::string& ) {return false;}

std::string MediaFile::getType() { return "";}

std::string MediaFile::getMetadata(const std::string&) const { return "";}

std::unordered_map<std::string, std::string> MediaFile::getAllMetadata() const { return {};}


