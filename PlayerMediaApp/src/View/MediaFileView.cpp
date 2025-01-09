#include "../../include/View/MediaFileView.h"

void MediaFileView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    // std::cout << "1. View metadata for a specific file" << std::endl;
    // std::cout << "2. View metadata for all files" << std::endl;
    // std::cout << "3. Exit" << std::endl;
    // std::cout << "=====================================" << std::endl;
    // std::cout << "Please choose an option: ";

    std::cout << "1. View Metadata" << std::endl;
    std::cout << "2. Edit Metadata" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

void MediaFileView::displayFileDetails(const MediaFile& media) {
    media.detailMediaFile();
}

void MediaFileView::displayFileDetails(const AudioFile& audio) {
    audio.detailMediaFile();
}

// void MediaFileView::showAllMediaFile(const MetadataManager &metadataManager) {
//     const auto &files = metadataManager.getMediaFiles();
//     std::cout << "\n========== All Media Files ==========" << std::endl;

//     for (const auto &file : files) {
//         std::cout << "------------------------------------" << std::endl;
//         showMetadataFile(*file);
//     }

//     std::cout << "=====================================\n" << std::endl;
// }

