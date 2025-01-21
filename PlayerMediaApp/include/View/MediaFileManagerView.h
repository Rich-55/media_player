#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "../View/BaseView.h"
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../Model/MediaFileManager.h"

class MediaFileManagerView : public BaseView{
private:

public:

    MediaFileManagerView();

    int showMenu() override;

    int showMenuWithMediaList(MediaFileManager) override;

    void showNotificationMessage(std::string, std::string) override;
    bool showConfirmMenu(std::string) override;
    
    std::string displayAllMediaFile(MediaFileManager) override;
    std::string  displayAllMediaFileOfAudio(MediaFileManager) override;
    std::string  displayAllMediaFileOfVideo(MediaFileManager) override;

};

#endif

