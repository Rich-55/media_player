#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include "../Model/MediaFile.h"  // Include MediaFile header file
#include "../Model/VideoFile.h"  // Include VideoFile header file
#include <iostream>

class MediaFileView {
public:
    void displayFileDetails(const MediaFile& media);
    void displayFileDetails(const VideoFile& video);
};

#endif

