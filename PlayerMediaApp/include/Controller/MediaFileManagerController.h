#ifndef MEDIA_FILE_MANAGER_CONTROLLER_H
#define MEDIA_FILE_MANAGER_CONTROLLER_H

#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include "../View/MetadataView.h"

#include <memory>
#include <unordered_set>
class MediaFileManagerController {
private:
    MetadataManager& mediaManager;
    std::shared_ptr<ViewBase> mediaFileManagerView; 

public:
    MediaFileManagerController(MetadataManager&, std::shared_ptr<ViewBase>);

    void addData(const std::unordered_set<std::string>&);

    void showAllMediaFile();

    void showAllMediaFileOfVideo();

    void showAllMediaFileOfAudio();

};

#endif 
