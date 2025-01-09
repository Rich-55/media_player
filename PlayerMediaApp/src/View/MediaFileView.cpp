#include "../../include/View/MediaFileView.h"

void MediaFileView::displayFileDetails(const MediaFile& media) {
    media.detailMediaFile();
}

void MediaFileView::displayFileDetails(const VideoFile& video) {
    video.detailMediaFile();
}
