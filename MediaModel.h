#ifndef MEDIAMODEL_H
#define MEDIAMODEL_H

#include <vector>
#include <string>

class MediaModel {
public:
    void loadMediaFiles(const std::string& directory);
    const std::vector<std::string>& getMediaFiles() const;

private:
    std::vector<std::string> mediaFiles;
    bool isSupportedFile(const std::string& file) const;
};

#endif
