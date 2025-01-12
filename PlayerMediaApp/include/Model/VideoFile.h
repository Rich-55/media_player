#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "MediaFile.h"

class VideoFile : public MediaFile {
private:
    std::unordered_map<std::string, std::string> metadataVideo;
    const static std::unordered_set<std::string> allowedKeys;
public:
    VideoFile();
    VideoFile(const std::string&, const std::string&, unsigned long,const std::string&, const std::string&);

    // Metadata operations
    std::string getMetadata(const std::string& key) const override;
    std::unordered_map<std::string, std::string> getAllMetadata() const override;

    // Overridden methods
    std::string getType() override;
    void inputMediaFile(std::string) override;

    void addNewKey(const std::string& key, const std::string& value) override;
    void editKey(const std::string& key, const std::string& value) override;
    void deleteKey(const std::string& key) override;
};

#endif
