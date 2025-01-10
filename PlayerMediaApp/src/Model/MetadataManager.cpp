#include "../../include/Model/MetadataManager.h"
#include <iostream>

MetadataManager::MetadataManager() {}

// MetadataManager class implementation (Model)
void MetadataManager::addMediaFile(std::shared_ptr<MediaFile> file) {
    listMediaFiles.push_back(file);
    std::cout << "Media file added: " << file->getName() << std::endl;
}

void MetadataManager::removeMediaFile(std::shared_ptr<MediaFile> file) {
    auto it = std::remove_if(listMediaFiles.begin(), listMediaFiles.end(),
        [&file](const std::shared_ptr<MediaFile>& media) {
            return media->getName() == file->getName();
        });
    if (it != listMediaFiles.end()) {
        listMediaFiles.erase(it, listMediaFiles.end());
        std::cout << "Removed media file: " << file->getName() << std::endl;
    } else {
        std::cout << "Media file not found: " << file->getName() << std::endl;
    }
}

void MetadataManager::editMediaFile(std::shared_ptr<MediaFile> file) {
    auto it = std::find_if(listMediaFiles.begin(), listMediaFiles.end(),
        [&file](const std::shared_ptr<MediaFile>& media) {
            return media->getName() == file->getName();
        });
    if (it != listMediaFiles.end()) {
        (*it)->editMediaFile();
        std::cout << "Edited media file: " << file->getName() << std::endl;
    } else {
        std::cout << "Media file not found: " << file->getName() << std::endl;
    }
}

void MetadataManager::displayAllMediaFiles() const {
    if (listMediaFiles.empty()) {
        std::cout << "No media files available." << std::endl;
        return;
    }

    std::cout << "List of media files:" << std::endl;
    for (const auto& file : listMediaFiles) {
        file->detailMediaFile();
    }
}
MetadataManager::~MetadataManager() {
    // Hàm hủy (nếu không có gì đặc biệt, có thể để trống)
}