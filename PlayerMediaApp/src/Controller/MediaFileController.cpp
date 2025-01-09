#include "../../include/Controller/MediaFileController.h"
#include "../../include/Model/MediaFile.h"   // Đảm bảo đường dẫn đúng
#include "../../include/View/MediaFileView.h"

MediaFileController::MediaFileController(MediaFile* m, MediaFileView& vView)
    : media(m), view(vView) {}

void MediaFileController::updateView() {
    view.displayFileDetails(*media);
}

//void MediaFileController::setData(int data) {
    // Logic for setting data for media or video

