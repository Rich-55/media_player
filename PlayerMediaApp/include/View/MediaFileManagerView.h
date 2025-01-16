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
    
    std::string displayAllMediaFile(MediaFileManager) override;
    void displayAllMediaFileOfAudio(MediaFileManager) override;
    void displayAllMediaFileOfVideo(MediaFileManager) override;

    


};

#endif

