#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include <iostream>
#include <vector>
#include <iomanip>
#include "../View/BaseView.h"
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../Model/MetadataManager.h"

class MediaFileManagerView : public BaseView{
private:

public:

    MediaFileManagerView();
    
    void displayAllMediaFile(MetadataManager) override;
    void displayAllMediaFileOfAudio(MetadataManager) override;
    void displayAllMediaFileOfVideo(MetadataManager) override;

    void showMenu() override;


};

#endif

