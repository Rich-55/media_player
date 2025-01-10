#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include "MediaFile.h"

class AudioFile : public MediaFile {
    private:
        std::string trackName;
        std::string album;
        std::string artist;
        std::string genre;
        int bitrate;
        int sampleRate;

    public:
        AudioFile();
        AudioFile(const std::string& , const std::string& , double , const std::string& , const std::string&,
                const std::string&, const std::string&, const std::string&, const std::string&, int, int);

        std::string getTrackname() const;
        std::string getAlbum() const;
        std::string getArtist() const;
        std::string getGenre() const;
        int getBitrate() const;
        int getSampleRate() const;

        std::string getType() override;
        
        void setTrackName(const std::string& newTrackName);
        void setAlbum(const std::string& newAlbum);
        void setGenre(const std::string& newGenre);
        void setArtist(const std::string& newArtist);

        void inputMediaFile(std::string) override;
        void detailMediaFile() const override;
        void editMediaFile() override;

        // static void addMetadataKey(TagLib::MPEG::File &file, const std::string &key, const std::string &value);
};

#endif
