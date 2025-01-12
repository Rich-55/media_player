#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>
#include <taglib/id3v2frame.h>
#include <taglib/mp4file.h>

#include <iostream>
#include <string>
#include <unordered_map>

class MediaFile {
private:
    std::string fileName;
    std::string pathName;
    unsigned long size;
    std::string duration;
    std::string fileType;
protected:
    void setType(std::string);
public:
    // Constructor
    MediaFile();
    MediaFile(const std::string& , const std::string& , unsigned long, const std::string& , const std::string& );


    // Virtual Destructor
    virtual ~MediaFile();

    // Getters
    std::string getName() const;
    std::string getPath() const;
    unsigned long getSize() const;
    std::string getDuration() const;

    virtual std::string getType();

    // Virtual methods
    virtual void inputMediaFile(std::string);
    
    virtual void addNewKey(const std::string& key, const std::string& value);
    virtual void editKey(const std::string& key, const std::string& value);
    virtual void deleteKey(const std::string& key);

    virtual std::string getMetadata(const std::string& key) const;
    virtual std::unordered_map<std::string, std::string> getAllMetadata() const;
    

};
#endif