#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include "../View/MetadataView.h"

#include <memory>
class MediaFileController {
private:
    MetadataManager mediaManager;
    std::shared_ptr<ViewBase> mediaView; 

public:
    MediaFileController(MetadataManager, std::shared_ptr<ViewBase>);

    void addData(std::vector<std::string>);

    void showAllMediaFile();

    void showAllAudioFile();

};

#endif 
