#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(MetadataManager m, std::shared_ptr<ViewBase> v) : mediaManager(m), mediaView(v){}

void MediaFileController::showAllMediaFile(){
    mediaView->displayAllMediaFile(mediaManager);
}

void MediaFileController::showAllAudioFile() {
    
    mediaView->displayAllMediaFileOfAudio(mediaManager);
}

void MediaFileController::addData(std::vector<std::string> listPathName){
    for(std::string path : listPathName){

        std::string check = "";
        size_t pos = path.rfind('.');
        if (pos != std::string::npos) {
            check = path.substr(pos); 
        }
        if(check == "mp4"){
            this->mediaManager.addMediaFile(path, "Video");
        } else {
            this->mediaManager.addMediaFile(path, "Audio");  
        }
    }
}

void MediaFileController::editMediaFile(std::string fileName) {
    std::cout << "Editting Media File: " << fileName <<std::endl;
    mediaManager.editMediaFile(fileName);
}

void MediaFileController::removeMediaFile(std::string fileName) {
    std::cout << "Removing Media File: " << fileName <<std::endl;
    mediaManager.removeMediaFile(fileName);
}
