#include "../../include/Model/VideoFile.h"

VideoFile::VideoFile() {}

VideoFile::VideoFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& dateCreated, const std::string& duration, const std::string& fileType)
    : MediaFile(fileName, pathName, size, dateCreated, duration, fileType) {}

std::string VideoFile::getMetadata(const std::string& key) const
 {
    if (metadataVideo.find(key) != metadataVideo.end()) {
        return metadataVideo.at(key);
    }
    return "";
}

std::map<std::string, std::string> VideoFile::getAllMetadata() const {return metadataVideo;}
void VideoFile::inputMediaFile(std::string pathName, bool isSame) {
    MediaFile::inputMediaFile(pathName, isSame);

    TagLib::FileRef file(pathName.c_str());

    if (file.isNull() || !file.tag()) {
        std::cerr << "Error opening file or accessing tags: " << pathName << std::endl;
        return;
    }

    TagLib::Tag* tag = file.tag();
    metadataVideo["title"] = tag->title().to8Bit(true).empty() ? "Unknown" : tag->title().to8Bit(true);
    metadataVideo["album"] = tag->album().to8Bit(true).empty() ? "Unknown" : tag->album().to8Bit(true);
    metadataVideo["artist"] = tag->artist().to8Bit(true).empty() ? "Unknown" : tag->artist().to8Bit(true);
    metadataVideo["year"] = tag->year() == 0 ? "Unknown" : std::to_string(tag->year());
    metadataVideo["comment"] = tag->comment().to8Bit(true).empty() ? "Unknown" : tag->comment().to8Bit(true);
    
    TagLib::AudioProperties* audioProperties = file.audioProperties();
    if (audioProperties) {
        metadataVideo["bitrate"] = std::to_string(audioProperties->bitrate());
    }

    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, pathName.c_str(), nullptr, nullptr) != 0) {
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        return;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        AVStream* stream = formatContext->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            int width = stream->codecpar->width;
            int height = stream->codecpar->height;
            const char* codecName = avcodec_get_name(stream->codecpar->codec_id);

            metadataVideo["resolution"] = std::to_string(width) + "x" + std::to_string(height);
            metadataVideo["codec"] = codecName ? codecName : "Unknown";
            break; 
        }
    }

    avformat_close_input(&formatContext);

    setType("Video");
}


bool VideoFile::addNewKey(const std::string& key, const std::string& value) {
    bool check = false;
    if (metadataVideo.find(key) != metadataVideo.end()) {
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
                check = false;
            }
        } else if (key == "track") {
            try {
                int track = std::stoi(value);
                tag->setTrack(track);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Track must be a valid number.\n";
                check = false;
            }
        }

        if (fileRef.save()) {
            metadataVideo[key] = value;
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

bool VideoFile::editKey(const std::string& key, const std::string& value) {
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
                check = false;
            }
        } else if (key == "track") {
            try {
                int track = std::stoi(value);
                tag->setTrack(track);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Track must be a valid number.\n";
                check = false;
            }
        }

        if (fileRef.save()) {
            metadataVideo[key] = value;
            check = true;
        } else {
            check = false;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid.\n";
        check = false;
    }
    return check;
}

bool VideoFile::deleteKey(const std::string& key) {
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
        }else if(key == "comment") {
            tag->setComment(TagLib::String("", TagLib::String::UTF8));
        }else {
            std::cerr << "Error: Unsupported metadata key [" << key << "]\n";
            check =  false; 
        }

        if (fileRef.save()) {
            std::cout << "Deleted metadata [" << key << "] (set to empty value) in ID3v2 tag.\n";
        } else {
            std::cerr << "Error: Failed to save changes to the file.\n";
        }

        if (metadataVideo.erase(key)) {
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

std::string VideoFile::getType() {return "Video";}
