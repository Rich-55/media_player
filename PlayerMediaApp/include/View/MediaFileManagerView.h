#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include "BaseView.h"

#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../Model/MediaFileManager.h"

class MediaFileManagerView : public BaseView{
private:

public:

    MediaFileManagerView();

    int showMenu() override;

    int showMenuWithMediaList(MediaFileManager mediaFileManager) override;

    void showNotificationMessage(std::string message, std::string type) override;
    bool showConfirmMenu(std::string message) override;
    
    std::string displayAllMediaFile(MediaFileManager MediaFileManager) override;
    std::string  displayAllMediaFileOfAudio(MediaFileManager MediaFileManager) override;
    std::string  displayAllMediaFileOfVideo(MediaFileManager MediaFileManager) override;

};

#endif

