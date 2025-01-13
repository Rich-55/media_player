#include "../../include/Controller/MediaFileManagerController.h"

MediaFileManagerController::MediaFileManagerController(MetadataManager& m, std::shared_ptr<ViewBase> v) : mediaManager(m), mediaFileManagerView(v){}


void MediaFileManagerController::addData(const std::unordered_set<std::string> &listPathName) {
    for (const auto &path : listPathName) {
        size_t lastSlashPos = path.find_last_of("/");
        std::string fileName = (lastSlashPos != std::string::npos) ? path.substr(lastSlashPos + 1) : path;

        size_t lastDotPos = path.find_last_of(".");
        std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

        if (extension == "mp4") {
            mediaManager.addMediaFile(path, "Video");
        } else if (extension == "mp3") {
            mediaManager.addMediaFile(path, "Audio");
        } else {
            std::cerr << "Unsupported file type: " << fileName << '\n';
            continue;
        }
    }
}

void MediaFileManagerController::deleteData(std::string fileName) {
    mediaManager.deleteMediaFile(fileName);
}

void MediaFileManagerController::showAllMediaFile(){
    
    mediaFileManagerView->displayAllMediaFile(mediaManager);
    
}

void MediaFileManagerController::showAllMediaFileOfVideo(){
    
    mediaFileManagerView->displayAllMediaFileOfVideo(mediaManager);
    
}

void MediaFileManagerController::showAllMediaFileOfAudio(){
    
    mediaFileManagerView->displayAllMediaFileOfAudio(mediaManager);
    
}

