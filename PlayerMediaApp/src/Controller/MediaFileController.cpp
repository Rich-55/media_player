#include "../../include/Controller/MediaFileController.h"
#include "../../include/Model/MediaFile.h"   // Đảm bảo đường dẫn đúng
#include "../../include/View/MediaFileView.h"

// MediaFileController class implementation (Controller)
MediaFileController::MediaFileController(MetadataManager& m, MediaFileView& v)
    : model(m), view(v) {}

void MediaFileController::updateView() {
    model.displayAllMediaFiles();
}

void MediaFileController::addMediaFile(std::shared_ptr<MediaFile> file) {
    model.addMediaFile(file);
}

void MediaFileController::removeMediaFile(std::shared_ptr<MediaFile> file) {
    model.removeMediaFile(file);
}

void MediaFileController::editMediaFile(std::shared_ptr<MediaFile> file) {
    model.editMediaFile(file);
}


