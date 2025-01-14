#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include <iostream>
#include <vector>
#include <iomanip>
#include "../View/BaseView.h"
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../Model/MediaFileManager.h"

class MediaFileManagerView : public BaseView{
private:

public:

    MediaFileManagerView();
    
    void displayAllMediaFile(MediaFileManager) override;
    void displayAllMediaFileOfAudio(MediaFileManager) override;
    void displayAllMediaFileOfVideo(MediaFileManager) override;

    void showMenu() override;


};

#endif

