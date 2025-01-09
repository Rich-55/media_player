#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include <iostream>
#include <vector>
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
// #include "../Model/MetadataManager.h"

class MediaFileView {
public:

    void showMenu();
    void displayFileDetails(const MediaFile& media);
    void displayFileDetails(const AudioFile& video);

};

#endif
