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
private:
    std::string fileName;
    std::string pathName;
    double size;
    std::string duration;
    std::string fileType;
protected:
    void setType(std::string);
public:
    // Constructor
    MediaFile();
    MediaFile(const std::string& , const std::string& , double , const std::string& , const std::string& );


    // Virtual Destructor
    virtual ~MediaFile();

    // Getters
    std::string getName() const;
    std::string getPath() const;
    double getSize() const;
    std::string getDuration() const;

    virtual std::string getType();
    virtual std::string getCodec();
    virtual int getBitrate();
    virtual std::string getResolution();

    // Virtual methods
    virtual void inputMediaFile(std::string);
    virtual void detailMediaFile() const;
    virtual void editMediaFile();
   // void editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue);

    // Getter for Audio
    virtual std::string getTrackname() const;
    virtual std::string getAlbum() const;
    virtual std::string getArtist() const;
    virtual std::string getGenre() const;
    virtual int getSampleRate() const;

};
#endif