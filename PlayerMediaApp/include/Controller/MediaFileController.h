#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "../Model/MediaFile.h"
#include "../Model/VideoFile.h"
#include "../View/MediaFileView.h"
#include "../Model/MetadataManager.h"

// MediaFileController (Controller)
class MediaFileController {
private:
    MetadataManager& model;
    MediaFileView& view;

public:
    MediaFileController(MetadataManager& m, MediaFileView& v);
    void updateView();
    void addMediaFile(std::shared_ptr<MediaFile> file);
    void removeMediaFile(std::shared_ptr<MediaFile> file);
    void editMediaFile(std::shared_ptr<MediaFile> file);
};

#endif 
