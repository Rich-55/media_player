#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include "../View/MetadataView.h"
#include "../utils/MetadataEditException.h"

#include <memory>
#include <unordered_set>
#include <limits>
#include <regex>

class MediaFileController {
private:
    std::shared_ptr<MediaFile> mediaFile;
    std::shared_ptr<ViewBase> mediaFileHandlerView; 
    const std::vector<std::string> allowedKeys = {
        "title", "artist", "album", "genre", "comment", "year", "track"
    };
    bool isKeyAllowed(const std::string& key) const;
    void validateYear(const std::string& year, const std::string& key) const;
    void validateStringNotEmpty(const std::string& value, const std::string& key) const;

public:
    MediaFileController(std::shared_ptr<MediaFile> , std::shared_ptr<ViewBase>);

    void getDetailMediaFile();

    void addMetadata();

    void editMetadata();

    void deleteMetadata();

};

#endif 
