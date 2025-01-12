#include "../../include/View/MediaFileManagerView.h"

MediaFileManagerView::MediaFileManagerView(){}

void MediaFileManagerView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    std::cout << "1. View All Media File" << std::endl;
    std::cout << "2. View All Audio Media File" << std::endl;
    std::cout << "3. View All Video Media File" << std::endl;
    std::cout << "0. Back to main menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

void MediaFileManagerView::displayAllMediaFile(MetadataManager metadataManager){
    if (metadataManager.getAllMediaFile().empty()) {
        std::cerr << "No media files to display.\n";
        return;
    }
    std::cout << "----- All Media Files -----" << std::endl;
    for (auto file : metadataManager.getAllMediaFile()) {
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << file->getType() << std::endl;
        std::cout << "File Size:   " << file->getSize() << " bytes" << std::endl;
        std::cout << "------------------------------" << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfAudio(MetadataManager metadataManager){
    if (metadataManager.getAllAudioFiles().empty()) {
        std::cerr << "No media files to display.\n";
        return;
    }
    std::cout << "----- All Audio Files -----" << std::endl;

    for (auto file : metadataManager.getAllAudioFiles()) {
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
        std::cout << std::string(50, '-') << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfVideo(MetadataManager metadataManager){
    if (metadataManager.getAllAudioFiles().empty()) {
        std::cerr << "No media files to display.\n";
        return;
    }
    std::cout << "----- All Video Files -----" << std::endl;
    for(auto file : metadataManager.getAllMediaFile()){
        if(file->getType() == "Video"){
            std::cout << "File Name:   " << file->getName() << std::endl;
            std::cout << "File Path:   " << file->getPath() << std::endl;
            std::cout << "File Type:   " << "Video File" << std::endl;
            std::cout << "File Size:   " << file->getSize() << std::endl;
            std::cout << "Duration:    " << file->getDuration() << " seconds" << std::endl;
            std::cout << "Codec:       " << file->getCodec() << std::endl;
            std::cout << "Bitrate:     " << file->getBitrate() << " kbps" << std::endl;
            std::cout << "Resolution:  " << file->getResolution() << std::endl;
            std::cout << std::string(50, '-') << std::endl;
        }
    }
}


