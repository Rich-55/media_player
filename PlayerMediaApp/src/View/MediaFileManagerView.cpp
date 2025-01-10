#include "../../include/View/MediaFileManagerView.h"

MediaFileManagerView::MediaFileManagerView(){}

void MediaFileManagerView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    std::cout << "1. View All Media File" << std::endl;
    std::cout << "2. View All Video Media File" << std::endl;
    std::cout << "3. View All Video Media File" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

void MediaFileManagerView::displayAllMediaFile(MetadataManager metadataManager){
    metadataManager.getAllMediaFile();
}

void MediaFileManagerView::displayAllMediaFileOfAudio(MetadataManager metadataManager){
    metadataManager.getAllMediaFile();
}

void MediaFileManagerView::displayAllMediaFileOfVideo(MetadataManager metadataManager){
    metadataManager.getAllMediaFile();
}


