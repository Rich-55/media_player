#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H


#include "../Model/MediaFile.h"
#include "../View/BaseView.h"
#include "../View/MetadataView.h"

#include "../utils/MetadataEditException.h"
#include "../utils/ConfigLoader.h"

#include <memory>
#include <unordered_set>
#include <regex>

class MediaFileController {
private:
    std::shared_ptr<MediaFile> mediaFile;
    std::shared_ptr<BaseView> mediaFileHandlerView; 
protected:
    bool isKeyAllowed(const std::string& key) const {
        std::unordered_set<std::string> keyAllowed = {
            "title",    // Title
            "artist",   // Artist
            "album",    // Album
            "genre",    // Genre
            "comment",  // Comment
            "year",     // Year
            "track"     // Track
        };
        return keyAllowed.find(key) != keyAllowed.end();
    }

    bool isValidFormat(const std::string& value, const std::string& pattern) {
        try {
            std::regex regexPattern(pattern);
            return std::regex_match(value, regexPattern);
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error: " << e.what() << std::endl;
            return false;
        }
    }
public:
    MediaFileController(std::shared_ptr<MediaFile> , std::shared_ptr<BaseView>);

    void handlerMediaFile();

    void getDetailMediaFile();

    void addMetadata();

    void editMetadata();

    void deleteMetadata();





};

#endif 
