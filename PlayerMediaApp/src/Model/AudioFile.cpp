#include "../../include/Model/AudioFile.h"

AudioFile::AudioFile() {};
AudioFile::AudioFile(const std::string& fileName, const std::string& pathName, double size, const std::string& duration, const std::string& fileType,
                    const std::string& trackName, const std::string& album, const std::string& artist, const std::string& genre, int bitrate, int sampleRate)
                    : MediaFile(fileName, pathName, size, duration, fileType), trackName(trackName), album(album), artist(artist), genre(genre), bitrate(bitrate), sampleRate(sampleRate) {}


std::string AudioFile::getTrackname() const { return trackName; }
std::string AudioFile::getAlbum() const { return album; }
std::string AudioFile::getArtist() const { return artist; }
std::string AudioFile::getGenre() const { return genre; }
int AudioFile::getBitrate() const { return bitrate; }
int AudioFile::getSampleRate() const { return sampleRate; }

std::string AudioFile::getType() const { return "Audio File"; }

void AudioFile::inputMediaFile(std::string pathName){
    MediaFile::inputMediaFile(pathName);

    TagLib::FileRef file(pathName.c_str());

    if (!file.isNull() && file.tag()) {
        std::cerr << "Error opening file or accessing tags: " << pathName << std::endl;  
    }

    TagLib::Tag *tag = file.tag();
    this->trackName = tag->title().to8Bit(true);
    this->album = tag->album().to8Bit(true);
    this->artist = tag->artist().to8Bit(true);
    this->genre = tag->genre().to8Bit(true);

    TagLib::AudioProperties *audioProperties = file.audioProperties();
    if (audioProperties) {
        this->bitrate = audioProperties->bitrate();
        this->sampleRate = audioProperties->sampleRate();
    }

    setType("Audio File");
   
}

void AudioFile::setTrackName(const std::string& newTrackName) {
    trackName = newTrackName;
}

void AudioFile::setAlbum(const std::string& newAlbum) {
    album = newAlbum;
}

void AudioFile::setGenre(const std::string& newGenre) {
    genre = newGenre;
}

void AudioFile::setArtist(const std::string& newArtist) {
    artist = newArtist;
}

void AudioFile::detailMediaFile() const {
    // MediaFile::detailMediaFile();

    std::cout << "----- Audio File Details -----" << std::endl;
    std::cout << "File Name:   " << this->getName() << std::endl;
    std::cout << "File Path:   " << this->getPath() << std::endl;
    std::cout << "File Type:   " << this->getType() << std::endl;
    std::cout << "File Size:   " << this->getSize() << std::endl;
    std::cout << "Track Name:  " << this->trackName << std::endl;
    std::cout << "Artist:      " << this->artist << std::endl;
    std::cout << "Album:       " << this->album << std::endl;
    std::cout << "Genre:       " << this->genre << std::endl;
    std::cout << "Duration:    " << this->getDuration() << " seconds" << std::endl;
    std::cout << "Bitrate:     " << this->bitrate << " kbps" << std::endl;
    std::cout << "Sample Rate: " << this->sampleRate << " Hz" << std::endl;
    std::cout << "------------------------------" << std::endl;
}


void AudioFile::editMediaFile() {
    TagLib::FileRef fileRef(this->getPath().c_str()); // Create FileRef locally

    if (!fileRef.isNull() && fileRef.tag()) {
        TagLib::Tag *tag = fileRef.tag();
        std::cout << "Editing metadata for: " << fileRef.file()->name() << std::endl;

        // Set metadata properties directly using TagLib::Tag methods:
        if (!trackName.empty()) {
            tag->setTitle(TagLib::String(trackName, TagLib::String::UTF8)); // Set Title
        }
        if (!artist.empty()) {
            tag->setArtist(TagLib::String(artist, TagLib::String::UTF8)); // Set Artist
        }
        if (!album.empty()) {
            tag->setAlbum(TagLib::String(album, TagLib::String::UTF8)); // Set Album
        }
        if (!genre.empty()) {
            tag->setGenre(TagLib::String(genre, TagLib::String::UTF8)); // Set Genre
        }

        // Save the changes
        if (!fileRef.save()) {
            std::cerr << "Error: Failed to save changes to the file." << std::endl;
        } else {
            std::cout << "Changes saved successfully." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not retrieve tag information or file is invalid." << std::endl;
    }
}
