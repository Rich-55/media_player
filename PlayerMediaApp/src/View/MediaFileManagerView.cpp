#include "../../include/View/MediaFileManagerView.h"

MediaFileManagerView::MediaFileManagerView(){
    // std::cout <<"added" << std::endl;
}

void MediaFileManagerView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    std::cout << "1. View All Media Files" << std::endl;
    std::cout << "2. View All Audio Files" << std::endl;
    std::cout << "3. View All Video Files" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

void MediaFileManagerView::displayAllMediaFile(MetadataManager metadataManager){
    if (metadataManager.getAllMediaFile().empty()) {
        std::cerr << "No media files to display.\n";
        return;
    }
    for (auto file : metadataManager.getAllMediaFile()) {
        std::cout << "----- Media Files -----" << std::endl;
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << file->getType() << std::endl;
        std::cout << "File Size:   " << file->getSize() << " bytes" << std::endl;
        std::cout << "------------------------------" << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfAudio(MetadataManager metadataManager){
    if (metadataManager.getAllAudioFiles().empty()) {
        std::cerr << "No audio file to display.\n";
        return;
    }
    for (auto file : metadataManager.getAllAudioFiles()) {
        std::cout << "----- Audio File Details -----" << std::endl;
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << "Audio File" << std::endl;
        std::cout << "File Size:   " << file->getSize() << std::endl;
        std::cout << "Track Name:  " << file->getTrackname() << std::endl;
        std::cout << "Artist:      " << file->getArtist() << std::endl;
        std::cout << "Album:       " << file->getAlbum() << std::endl;
        std::cout << "Genre:       " << file->getGenre() << std::endl;
        std::cout << "Duration:    " << file->getDuration() << " seconds" << std::endl;
        std::cout << "Bitrate:     " << file->getBitrate() << " kbps" << std::endl;
        std::cout << "Sample Rate: " << file->getSampleRate() << " Hz" << std::endl;
        std::cout << "------------------------------" << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfVideo(MetadataManager metadataManager){
    metadataManager.getAllMediaFile();
}


