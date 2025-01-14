#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "../Model/MetadataManager.h"
#include "../View/BaseView.h"
#include "../View/MetadataView.h"

#include <memory>
#include <unordered_set>

class MediaFileController {
private:
    std::shared_ptr<MediaFile> mediaFile;
    std::shared_ptr<BaseView> mediaFileHandlerView; 

public:
    MediaFileController(std::shared_ptr<MediaFile> , std::shared_ptr<BaseView>);

    void getDetailMediaFile();

    void addMetadata();

    void editMetadata();

    void deleteMetadata();





};

#endif 
