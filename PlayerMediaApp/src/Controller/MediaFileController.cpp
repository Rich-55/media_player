#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(MetadataManager m, std::shared_ptr<ViewBase> v) : mediaManager(m), mediaView(v){}


void MediaFileController::addData(std::vector<std::string> listPathName){
    for(std::string v : listPathName){

        std::string check = "";
        size_t pos = v.rfind('.');
        if (pos != std::string::npos) {
            check = v.substr(pos); 
        }
        if(check == "mp4"){
            this->mediaManager.addMediaFile(v, "Video");
        }
    }
}

void MediaFileController::showMediaFile(){
    std::shared_ptr<ViewBase> view = std::make_shared<MetadataView>();
    view->showMenu();
}

//void MediaFileController::setData(int data) {
    // Logic for setting data for media or video

