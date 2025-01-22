#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include "MediaFile.h"

class VideoFile : public MediaFile {
private:
    std::map<std::string, std::string> metadataVideo;
public:
    VideoFile();
    VideoFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& dateCreated, const std::string& duration, const std::string& fileType);

    // Metadata operations
    std::string getMetadata(const std::string& key) const override;
    std::map<std::string, std::string> getAllMetadata() const override;

    // Overridden methods
    std::string getType() override;
    void inputMediaFile(std::string pathName, bool isSame) override;

    bool addNewKey(const std::string& key, const std::string& value) override;
    bool editKey(const std::string& key, const std::string& value) override;
    bool deleteKey(const std::string& key) override;
};

#endif
