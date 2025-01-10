#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include <iostream>
#include <vector>
#include "../View/ViewBase.h"
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../Model/MetadataManager.h"

class MediaFileManagerView : public ViewBase{
private:

public:

    MediaFileManagerView();
    
    void displayAllMediaFile(MetadataManager);

    void displayAllMediaFileOfAudio(MetadataManager);

    void displayAllMediaFileOfVideo(MetadataManager);

    void showMenu() override;


};

#endif

