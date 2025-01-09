#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <limits> 

#include "include/Model/MediaFile.h"
#include "include/Model/AudioFile.h"
#include "include/Controller/MediaFileController.h"
#include "include/View/MediaFileView.h"

namespace fs = std::filesystem;
class MediaScanner {
public:
    void scan(const std::string& path) {
        mediaFiles.clear();
        try {
            if (!fs::exists(path) || !fs::is_directory(path)) {
                throw std::runtime_error("Invalid path or not a directory.");
            }

            for (const auto& entry : fs::recursive_directory_iterator(path)) {
                if (entry.is_regular_file() && isMediaFile(entry.path())) {
                    mediaFiles.push_back(entry.path().string());
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error while scanning: " << e.what() << std::endl;
        }
    }

    const std::vector<std::string>& getMediaFiles() const {
        return mediaFiles;
    }

private:
    std::vector<std::string> mediaFiles;

    bool isMediaFile(const fs::path& path) {
        // Example check for file extension, adjust as needed
        return path.extension() == ".mp4" || path.extension() == ".mp3";
    }
};


int main() {
    MediaScanner scanner;
    std::string path;

    std::cout << "Enter the directory path to scan: ";
    std::getline(std::cin, path);

    scanner.scan(path);

    const std::vector<std::string>& files = scanner.getMediaFiles();

    MediaFile *audio = new AudioFile;
    audio->inputMediaFile(files[0].c_str());

    // Create view and controller
    MediaFileView view;
    MediaFileController controller(audio, view);

    // Update view
    controller.updateView();


    controller.setTrackName("Khong Phai Nhac");
    controller.setAlbum("New Album Name");
    controller.setArtist("New Artist Name");
    controller.setGenre("New Genre");
    
    // This is a must to modify metadata
    controller.saveChanges();

    controller.updateView();

    // Clean up
    delete audio;

    return 0;

}
