#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include "../View/MetadataView.h"

#include <memory>
#include <unordered_set>

class MediaFileController {
private:
    std::shared_ptr<MediaFile> mediaFile;
    std::shared_ptr<ViewBase> mediaFileHandlerView; 

public:
    MediaFileController(std::shared_ptr<MediaFile> , std::shared_ptr<ViewBase>);

    void getDetailMediaFile();

    void addMetadata();

    void editMetadata();

    void deleteMetadata();





};

#endif 
