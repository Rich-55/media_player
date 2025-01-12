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
    MediaFile(const std::string& , const std::string& , double , const std::string&, const std::string&);

    // Virtual Destructor
    virtual ~MediaFile();

    // Getters
    std::string getName() const;
    std::string getPath() const;
    double getSize() const;
    std::string getDuration() const;

    // Virtual method to get type
    virtual std::string getType();
    virtual std::string getCodec ();
    virtual int getBitrate ();
    virtual std::string getResolution ();

    // Getters for metadata
    virtual std::string getTitle() const;
    virtual std::string getArtist() const;
    virtual std::string getAlbum() const;
    virtual int getYear() const;

    // Virtual methods for file operations
    virtual void inputMediaFile(std::string);
    virtual void detailMediaFile() const;
    virtual void editMediaFile();

    virtual void addNewKey(const std::string& key, const std::string& value) = 0;
    virtual void editKey(const std::string& key, const std::string& newValue) = 0;
    virtual void deleteKey(const std::string& key) = 0;
};
   // void editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue);

#endif
