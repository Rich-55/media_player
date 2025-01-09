#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>

#include <iostream>
#include <string>

class MediaFile {
protected:
    std::string fileName;
    std::string pathName;
    double size;
    std::string duration;
    std::string fileType;

public:
    // Constructor
    MediaFile(std::string fileName, std::string pathName, double size, std::string duration, std::string fileType);
    // Virtual Destructor
    virtual ~MediaFile();

    // Getters
    std::string getName() const;
    std::string getPath() const;
    int getDuration() const;
    virtual std::string getType() const;

    // Virtual methods
    virtual void detailMediaFile() const;
    virtual void editMediaFile();
    void editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue);

};
#endif