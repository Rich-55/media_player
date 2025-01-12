#include "../../include/Model/AudioFile.h"

const std::unordered_set<std::string> AudioFile::allowedKeys = {
        "title",    // Title
        "artist",   // Artist
        "album",    // Album
        "genre",    // Genre
        "comment",  // Comment
        "year",     // Year
        "track"     // Track
};


AudioFile::AudioFile() {};

AudioFile::AudioFile(const std::string& fileName, const std::string& pathName, unsigned long size, const std::string& duration, const std::string& fileType)
    : MediaFile(fileName, pathName, size, duration, fileType) {}

// Lấy giá trị metadata theo key
std::string AudioFile::getMetadata(const std::string& key) const {
    if (metadataAudio.find(key) != metadataAudio.end()) {
        return metadataAudio.at(key);
    }
    return "";
}

std::unordered_map<std::string, std::string> AudioFile::getAllMetadata() const {
    return metadataAudio;
}


void AudioFile::inputMediaFile(std::string pathName) {
    MediaFile::inputMediaFile(pathName);

    TagLib::FileRef file(pathName.c_str());

    if (file.isNull() || !file.tag()) {
        std::cerr << "Error opening file or accessing tags: " << pathName << std::endl;
        return;
    }

    TagLib::Tag* tag = file.tag();
    metadataAudio["trackName"] = tag->title().to8Bit(true).empty() ? "Unknown" : tag->title().to8Bit(true);
    metadataAudio["album"] = tag->album().to8Bit(true).empty() ? "Unknown" : tag->album().to8Bit(true);
    metadataAudio["artist"] = tag->artist().to8Bit(true).empty() ? "Unknown" : tag->artist().to8Bit(true);
    metadataAudio["genre"] = tag->genre().to8Bit(true).empty() ? "Unknown" : tag->genre().to8Bit(true);

    TagLib::AudioProperties* audioProperties = file.audioProperties();
    if (audioProperties) {
        metadataAudio["bitrate"] = std::to_string(audioProperties->bitrate());
        metadataAudio["sampleRate"] = std::to_string(audioProperties->sampleRate());
    }

    setType("Audio");
}

void AudioFile::addNewKey(const std::string& key, const std::string& value) {

    if (allowedKeys.find(key) == allowedKeys.end()) {
        std::cerr << "Error: Unsupported metadata key: " << key << "\n";
        return;
    }

    if (metadataAudio.find(key) != metadataAudio.end()) {
        std::cerr << "Error: Metadata key [" << key << "] already exists.\n";
        return;
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
                return;
            }
        } else if (key == "track") {
            try {
                int track = std::stoi(value);
                tag->setTrack(track);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Track must be a valid number.\n";
                return;
            }
        }

        if (fileRef.save()) {
            metadataAudio[key] = value;
            std::cout << "Added new metadata [" << key << "] with value: " << value << "\n";
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid.\n";
    }
}

void AudioFile::editKey(const std::string& key, const std::string& value) {

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
                return;
            }
        } else if (key == "track") {
            try {
                int track = std::stoi(value);
                tag->setTrack(track);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Track must be a valid number.\n";
                return;
            }
        }

        if (fileRef.save()) {
            metadataAudio[key] = value;
            std::cout << "Updated metadata [" << key << "] to: " << value << "\n";
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid.\n";
    }
}

void AudioFile::deleteKey(const std::string& key) {
    if (allowedKeys.find(key) == allowedKeys.end()) {
        std::cerr << "Error: Unsupported metadata key: " << key << "\n";
        return;
    }

    if (metadataAudio.find(key) != metadataAudio.end()) {
        std::cerr << "Error: Metadata key [" << key << "] already exists.\n";
        return;
    }

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
        } else {
            std::cerr << "Error: Unsupported metadata key [" << key << "]\n";
            return;
        }

        if (fileRef.save()) {
            std::cout << "Deleted metadata [" << key << "] (set to empty value) in ID3v2 tag.\n";
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
        }

        if (metadataAudio.erase(key)) {
            std::cout << "Deleted metadata [" << key << "] from map.\n";
        } else {
            std::cerr << "Error: Metadata key [" << key << "] does not exist in audio.\n";
        }
    } else {
        std::cerr << "Error: Unable to modify tag for the file.\n";
    }
}


std::string AudioFile::getType() {
    return "Audio";
}
