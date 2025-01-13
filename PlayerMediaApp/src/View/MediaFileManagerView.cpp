#include "../../include/View/MediaFileManagerView.h"

MediaFileManagerView::MediaFileManagerView(){}

void MediaFileManagerView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    std::cout << "1. Add File By File Path" << std::endl;
    std::cout << "2. Add File By Folder Path" << std::endl;
    std::cout << "3. Delete File" << std::endl;
    std::cout << "4. View All Media File" << std::endl;
    std::cout << "5. View All Audio Media File" << std::endl;
    std::cout << "6. View All Video Media File" << std::endl;
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
        std::cout << "File Duration:   " << file->getDuration() << std::endl;
        std::cout << "------------------------------" << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfAudio(MetadataManager metadataManager) {
    auto audioFiles = metadataManager.getAllAudioFiles();
    if (audioFiles.empty()) {
        std::cerr << "No audio files to display.\n";
        return;
    }

    std::cout << "----- All Audio Files -----" << std::endl;
    for (const auto& file : audioFiles) {
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << file->getType() << std::endl;
        std::cout << "File Size:   " << file->getSize() << " bytes" << std::endl;

        std::cout << "Audio Metadata:" << std::endl;
        for (const auto& [key, value] : file->getAllMetadata()) {
            std::cout << "  " << key << ": " << value << std::endl;
        }

        std::cout << std::string(50, '-') << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfVideo(MetadataManager metadataManager) {
    auto videoFiles = metadataManager.getAllVideoFiles();
    if (videoFiles.empty()) {
        std::cerr << "No video files to display.\n";
        return;
    }

    std::cout << "----- All Video Files -----" << std::endl;
    for (const auto& file : videoFiles) {
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << file->getType() << std::endl;
        std::cout << "File Size:   " << file->getSize() << " bytes" << std::endl;

        std::cout << "Video Metadata:" << std::endl;
        for (const auto& [key, value] : file->getAllMetadata()) {
            std::cout << "  " << key << ": " << value << std::endl;
        }

        std::cout << std::string(50, '-') << std::endl;
    }
}

