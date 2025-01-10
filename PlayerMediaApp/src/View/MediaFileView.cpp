#include "../../include/View/MediaFileView.h"

// MediaFileView class implementation (View)
void MediaFileView::displayFileDetails(const MediaFile& media) {
    media.detailMediaFile();
}

void MediaFileView::displayFileDetails(const VideoFile& video) {
    video.detailMediaFile();
}