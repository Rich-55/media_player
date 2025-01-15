#include "MediaModel.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void MediaModel::loadMediaFiles(const std::string& directory) {
    std::cout << "Scanning directory: " << directory << "\n";
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::cout << "Found file: " << path << "\n"; // Debug output
            if (isSupportedFile(path)) {
                std::cout << "Adding supported file: " << path << "\n"; // Debug output
                mediaFiles.push_back(path);
            }
        }
    }
}

const std::vector<std::string>& MediaModel::getMediaFiles() const {
    return mediaFiles;
}

bool MediaModel::isSupportedFile(const std::string& file) const {
    return file.size() >= 4 && 
           (file.substr(file.size() - 4) == ".mp3" || file.substr(file.size() - 4) == ".mp4");
}
