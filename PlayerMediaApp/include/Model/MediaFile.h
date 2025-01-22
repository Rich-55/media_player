#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include "../utils/ConfigLoader.h"

class MediaFile {
private:
    std::string fileName;
    std::string pathName;
    unsigned long long size;
    std::string dateCreated;
    std::string duration;
    std::string fileType;
protected:
    void setType(std::string);
public:
    // Constructor
    MediaFile();
    MediaFile(const std::string& fileName, const std::string& pathName, unsigned long long size, 
                const std::string& dateCreated, const std::string& duration, const std::string& fileType);
    // Virtual Destructor
    virtual ~MediaFile();

    // Getters
    std::string getName() const;
    std::string getPath() const;
    unsigned long getSize() const;
    std::string getDuration() const;
    std::string getDateCreated() const;

    virtual std::string getType();

    // Virtual methods
    virtual void inputMediaFile(std::string, bool);
    
    virtual bool addNewKey(const std::string& key, const std::string& value);
    virtual bool editKey(const std::string& key, const std::string& value);
    virtual bool deleteKey(const std::string& key);

    virtual std::string getMetadata(const std::string& key) const;
    virtual std::map<std::string, std::string> getAllMetadata() const;
    

};
#endif