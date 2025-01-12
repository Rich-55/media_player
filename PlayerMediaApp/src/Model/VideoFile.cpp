#include "../../include/Model/VideoFile.h"

VideoFile::VideoFile(){}
// Constructor
VideoFile::VideoFile(const std::string& fileName, const std::string& pathName, double size, const std::string& duration, const std::string& fileType,
               const std::string& codec, int bitrate, const std::string& resolution, const std::string& title, const std::string& album, const std::string& artist, int year)
             : MediaFile(fileName, pathName, size, duration, fileType), codec(codec), bitrate(bitrate), resolution(resolution), title(title), album(album), artist(artist), year(year){}   

// Getters
std::string VideoFile::getCodec() { return codec; }
int VideoFile::getBitrate() { return bitrate; }
std::string VideoFile::getResolution() { return resolution; }
std::string VideoFile::getTitle() const { return title; }
std::string VideoFile::getAlbum() const { return album; }
std::string VideoFile::getArtist() const { return artist; }
int VideoFile::getYear() const { return year; }

std::string VideoFile::getType() { return "Video File"; }

void VideoFile::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void VideoFile::setAlbum(const std::string& newAlbum) {
    album = newAlbum;
}

void VideoFile::setYear(const int newYear) {
    year = newYear;
}

void VideoFile::setArtist(const std::string& newArtist) {
    artist = newArtist;
}


// Phương thức thêm một key mới vào metadata (lưu vào comment hoặc trường khác)
void VideoFile::addNewKey(const std::string& key, const std::string& value) {
    TagLib::FileRef fileRef(this->getPath().c_str());
    
    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag* tag = fileRef.tag();
        
        // Bạn có thể sử dụng comment để lưu các key tùy chỉnh (hoặc các trường khác như title, album...)
        if (key == "Genre") {
            tag->setComment(TagLib::String(value, TagLib::String::UTF8)); // Lưu vào trường comment
        }
        std::cout << "Added new key: " << key << " with value: " << value << std::endl;

        // Save changes
        if (!fileRef.save()) {
            std::cerr << "Error: Failed to save changes to the file." << std::endl;
        } else {
            std::cout << "Changes saved successfully." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid." << std::endl;
    }
}

// Phương thức sửa đổi giá trị của một key có sẵn trong metadata
void VideoFile::editKey(const std::string& key, const std::string& newValue) {
    TagLib::FileRef fileRef(this->getPath().c_str());
    
    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag* tag = fileRef.tag();
        
        // Sửa giá trị của key
        if (key == "Title") {
            tag->setTitle(TagLib::String(newValue, TagLib::String::UTF8));
        } else if (key == "Artist") {
            tag->setArtist(TagLib::String(newValue, TagLib::String::UTF8));
        } else if (key == "Album") {
            tag->setAlbum(TagLib::String(newValue, TagLib::String::UTF8));
        } else if (key == "Genre") {
            tag->setComment(TagLib::String(newValue, TagLib::String::UTF8)); // Sửa giá trị trong comment
        }
        
        std::cout << "Edited key: " << key << " to new value: " << newValue << std::endl;

        // Save changes
        if (!fileRef.save()) {
            std::cerr << "Error: Failed to save changes to the file." << std::endl;
        } else {
            std::cout << "Changes saved successfully." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid." << std::endl;
    }
}

// Phương thức xóa một key khỏi metadata
void VideoFile::deleteKey(const std::string& key) {
    TagLib::FileRef fileRef(this->getPath().c_str());
    
    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag* tag = fileRef.tag();
        
        // Xóa giá trị của key
        if (key == "Title") {
            tag->setTitle(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "Artist") {
            tag->setArtist(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "Album") {
            tag->setAlbum(TagLib::String("", TagLib::String::UTF8));
        } else if (key == "Genre") {
            tag->setComment(TagLib::String("", TagLib::String::UTF8)); // Xóa giá trị trong comment
        }
        
        std::cout << "Deleted key: " << key << std::endl;

        // Save changes
        if (!fileRef.save()) {
            std::cerr << "Error: Failed to save changes to the file." << std::endl;
        } else {
            std::cout << "Changes saved successfully." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid." << std::endl;
    }
}


void VideoFile::inputMediaFile(std::string pathName){
    MediaFile::inputMediaFile(pathName);
    TagLib::FileRef file(pathName.c_str());

      if (file.isNull() || !file.tag()) {
        std::cerr << "Error opening file or accessing tags: " << pathName << std::endl;
        return;
    }
    TagLib::Tag *tag = file.tag();
    this->title= tag->title().to8Bit(true);
    this->album = tag->album().to8Bit(true);
    this->artist = tag->artist().to8Bit(true);
    this->year = tag->year();

    // TagLib::AudioProperties *audioProperties = file.audioProperties();
    //   if (audioProperties) {
    //     this->bitrate = audioProperties->bitrate();
    //     this->resolution = audioProperties->resolution();
    //     this->codec = audioProperties->codec();
    // }

    // setType("Audio File");
}


// Overridden methods
void VideoFile::detailMediaFile() const {
    // MediaFile::detailMediaFile();
    std::cout << "----- Video File Details -----" << std::endl;
    std::cout << "File Name:   " << this->getName() << std::endl;
    std::cout << "File Path:   " << this->getPath() << std::endl;
    std::cout << "File Size:   " << this->getSize() << std::endl;
     std::cout << "Duration:    " << this->getDuration() << " seconds" << std::endl;
    std::cout << "File Type:   " << "Video File" << std::endl;

    std::cout << "Codec:  " << this->codec << std::endl;
    std::cout << "Artist:      " << this->artist << std::endl;
    std::cout << "Album:       " << this->album << std::endl;
    std::cout << "Resolution:       " << this->resolution << std::endl;
    std::cout << "Bitrate:     " << this->bitrate << " kbps" << std::endl;
    std::cout << "Year: " << this->year << std::endl;
    std::cout << "------------------------------" << std::endl;
}


// void VideoFile::editMediaFile() {
//     TagLib::FileRef fileRef(this->getPath().c_str()); // Create FileRef locally

//     if (!fileRef.isNull() && fileRef.tag()) {
//         TagLib::Tag *tag = fileRef.tag();
//         std::cout << "Editing metadata for: " << this->getPath() << std::endl;

//         // Set metadata properties directly using TagLib::Tag methods:
//         if (!title.empty()) {
//             tag->setTitle(TagLib::String(title, TagLib::String::UTF8)); // Set Title
//         }
//         if (!artist.empty()) {
//             tag->setArtist(TagLib::String(artist, TagLib::String::UTF8)); // Set Artist
//         }
//         if (!album.empty()) {
//             tag->setAlbum(TagLib::String(album, TagLib::String::UTF8)); // Set Album
//         }
//         if (year != 0) {
//             tag->setYear(year); // Set Year
//         }

//         // Save the changes
//         if (!fileRef.save()) {
//             std::cerr << "Error: Failed to save changes to the file." << std::endl;
//         } else {
//             std::cout << "Changes saved successfully." << std::endl;
//         }
//     } else {
//         std::cerr << "Error: Could not retrieve tag information or file is invalid." << std::endl;
//     }
// }
