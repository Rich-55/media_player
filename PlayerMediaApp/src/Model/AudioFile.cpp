#include "../../include/Model/AudioFile.h"

AudioFile::AudioFile() {};

AudioFile::AudioFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& dateCreated, const std::string& duration, const std::string& fileType)
    : MediaFile(fileName, pathName, size, dateCreated, duration, fileType) {}

// Lấy giá trị metadata theo key
std::string AudioFile::getMetadata(const std::string& key) const 
{
    if (metadataAudio.find(key) != metadataAudio.end()) {
        return metadataAudio.at(key);
    }
    return "";
}

std::map<std::string, std::string> AudioFile::getAllMetadata() const
{
    return metadataAudio;
}


void AudioFile::inputMediaFile(std::string pathName, bool isSame) 
{
    MediaFile::inputMediaFile(pathName, isSame);

    TagLib::FileRef file(pathName.c_str());

    if (file.isNull() || !file.tag()) {
        std::cerr << "Error opening file or accessing tags: " << pathName << std::endl;
        return;
    }

    TagLib::Tag* tag = file.tag();
    metadataAudio["track"] = tag->title().to8Bit(true).empty() ? "Unknown" : tag->title().to8Bit(true);
    metadataAudio["album"] = tag->album().to8Bit(true).empty() ? "Unknown" : tag->album().to8Bit(true);
    metadataAudio["artist"] = tag->artist().to8Bit(true).empty() ? "Unknown" : tag->artist().to8Bit(true);
    metadataAudio["genre"] = tag->genre().to8Bit(true).empty() ? "Unknown" : tag->genre().to8Bit(true);
    metadataAudio["comment"] = tag->comment().to8Bit(true).empty() ? "Unknown" : tag->comment().to8Bit(true);
    metadataAudio["year"] = tag->year() == 0 ? "Unknown" : std::to_string(tag->year());
    metadataAudio["title"] = tag->title().to8Bit(true).empty() ? "Unknown" : tag->title().to8Bit(true);
    TagLib::AudioProperties* audioProperties = file.audioProperties();
    if (audioProperties) {
        metadataAudio["bitrate"] = std::to_string(audioProperties->bitrate());
        metadataAudio["sampleRate"] = std::to_string(audioProperties->sampleRate());
    }

    setType("Audio");
}

bool AudioFile::addNewKey(const std::string& key, const std::string& value) 
{
    bool check = false;
    //check key is exist in metadataAudio
    if (metadataAudio.find(key) != metadataAudio.end()) {
        return check;
    }

    TagLib::FileRef fileRef(this->getPath().c_str());
    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag* tag = fileRef.tag();

        if (key == "title") {
            tag->setTitle(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "artist") {
            tag->setArtist(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "album") {
            tag->setAlbum(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "genre") {
            tag->setGenre(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "comment") {
            tag->setComment(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "year") {
            try {
                int year = std::stoi(value);
                tag->setYear(year);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Year must be a valid number.\n";
                return check;
            }
        } else if (key == "track") {
            try {
                int track = std::stoi(value);
                tag->setTrack(track);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Track must be a valid number.\n";
                return check;
            }
        }

        if (fileRef.save()) {
            metadataAudio[key] = value;
            check = true;
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
            check = false;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid.\n";
        check = false;
    }
    return check;
}

bool AudioFile::editKey(const std::string& key, const std::string& value) 
{
    bool check = false;
    TagLib::FileRef fileRef(this->getPath().c_str());
    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag* tag = fileRef.tag();

        if (key == "title") {
            tag->setTitle(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "artist") {
            tag->setArtist(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "album") {
            tag->setAlbum(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "genre") {
            tag->setGenre(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "comment") {
            tag->setComment(TagLib::String(value, TagLib::String::UTF8));
        } else if (key == "year") {
            try {
                int year = std::stoi(value);
                tag->setYear(year);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Year must be a valid number.\n";
                return check;
            }
        } else if (key == "track") {
            try {
                int track = std::stoi(value);
                tag->setTrack(track);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Track must be a valid number.\n";
                return check;
            }
        }

        if (fileRef.save()) {
            metadataAudio[key] = value;
            check = true;
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
            check = false;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid.\n";
        check = false;
    }
    return check;
}

bool AudioFile::deleteKey(const std::string& key) 
{
    bool check = false;

    TagLib::FileRef fileRef(this->getPath().c_str());
    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag* tag = fileRef.tag();

        if (key == "title") {
            tag->setTitle(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "album") {
            tag->setAlbum(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "artist") {
            tag->setArtist(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "genre") {
            tag->setGenre(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "year") {
            tag->setYear(0); 
        } else if (key == "track") {
            tag->setTrack(0); 
        } else if(key == "comment"){
            tag->setComment(TagLib::String("", TagLib::String::UTF8));
        }else {
            std::cerr << "Error: Unsupported metadata key [" << key << "]\n";
            check = false;
        }

        if (fileRef.save()) {
            std::cout << "Deleted metadata [" << key << "] (set to empty value) in ID3v2 tag.\n";
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
        }

        if (metadataAudio.erase(key)) {
            check = true;
        } else {
            check = false;
        }
    } else {
        std::cerr << "Error: Unable to modify tag for the file.\n";
        check = false;
    }
    return check;
}


std::string AudioFile::getType() {return "Audio";}
