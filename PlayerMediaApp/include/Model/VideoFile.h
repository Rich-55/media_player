#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include "MediaFile.h"

class VideoFile : public MediaFile {
private:
    std::map<std::string, std::string> metadataVideo;
    const static std::unordered_set<std::string> allowedKeys;
public:
    VideoFile();
    VideoFile(const std::string&, const std::string&, unsigned long long,const std::string&, const std::string&);

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
