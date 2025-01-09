#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "../Model/MediaFile.h"
#include "../Model/VideoFile.h"
#include "../View/MediaFileView.h"

class MediaFileController {
private:
    MediaFile* media; // tao 1 con tro ten media kieu MediaFile
    MediaFileView& view; // 

public:
    MediaFileController(MediaFile* m, MediaFileView& vView);
    void updateView();
    void setData(int data);
};

#endif 
