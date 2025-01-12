#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>

#include "MediaFile.h"

class VideoFile : public MediaFile {
private:
    std::string codec;
    int bitrate;
    std::string resolution;

    std::string title;
    std::string album;
    std::string artist;
    int year;

public:
    VideoFile();
    VideoFile (const std::string&, const std::string&, double, const std::string&, const std::string&,
               const std::string&, int, const std::string&, const std::string&, const std::string&, const std::string&, int);


   // Override getter methods for metadata
    std::string getCodec() override;
    int getBitrate() override;
    std::string getResolution() override;

    std::string getTitle() const override;
    std::string getArtist() const override;
    std::string getAlbum() const override;
    int getYear() const override;

    std::string getType() override;

    // Override setter methods for metadata
    void setTitle(const std::string& newTitle);
    void setArtist(const std::string& newArtist);
    void setAlbum(const std::string& newAlbum);
    void setYear(const int newYear);


    void inputMediaFile(std::string) override;
    void detailMediaFile() const override;
    void editMediaFile() override;


    void addNewKey(const std::string& key, const std::string& value) override;
    void editKey(const std::string& key, const std::string& newValue) override;
    void deleteKey(const std::string& key) override;
    
};

#endif