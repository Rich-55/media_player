#include "../../include/Model/MediaFile.h"

MediaFile::MediaFile(){}
MediaFile::MediaFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& dateCreated, const std::string& duration, const std::string& fileType)
    : fileName(fileName), pathName(pathName), size(size), dateCreated(dateCreated), duration(duration), fileType(fileType) {}

MediaFile::~MediaFile() {}

std::string MediaFile::getFileName() const { return this->fileName;}
std::string MediaFile::getPath() const { return this->pathName;}
unsigned long MediaFile::getSize() const { return this->size;}
std::string MediaFile::getDuration() const { return this->duration;}
std::string MediaFile::getDateCreated() const { return this->dateCreated;}
void MediaFile::setType(std::string type) { this->fileType = type;}

void MediaFile::inputMediaFile(std::string pathName, bool isSame) 
{
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
            this->fileName.insert(dotPos, "(another size)");
        } else {
            this->fileName += "(another size)";
        }
    }

    this->pathName = pathName;

    TagLib::MP4::File fileTemp(pathName.c_str());
    this->size = fileTemp.length();

    this->duration = std::to_string(audioProps->length() / 60) + "m " +
                     std::to_string(audioProps->length() % 60) + "s";
    try {
        auto ftime = std::filesystem::last_write_time(pathName);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        ftime - std::filesystem::file_time_type::clock::now() +
                        std::chrono::system_clock::now());
        auto time = std::chrono::system_clock::to_time_t(sctp);
        this->dateCreated = std::string(std::ctime(&time)); 
        this->dateCreated.pop_back(); 
    } catch (const std::exception &e) {
        std::cerr << "Error retrieving file creation date: " << e.what() << std::endl;
        this->dateCreated = "Unknown";
    }
}

bool MediaFile::addNewKey(const std::string&, const std::string&){return false;}

bool MediaFile::editKey(const std::string&, const std::string&) {return false;}

bool MediaFile::deleteKey(const std::string& ) {return false;}

std::string MediaFile::getType() { return "";}

std::string MediaFile::getMetadata(const std::string&) const { return "";}

std::map<std::string, std::string> MediaFile::getAllMetadata() const { return {}; }
