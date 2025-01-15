#include <iostream>
#include "MediaModel.h"
#include "MediaController.h"
#include "MediaView.h"

int main() {
    MediaModel model;
    std::string directory;

    std::cout << "Enter the path to the directory containing media files: ";
    int cho;
    std::cin >> cho;
    if(cho == 0) { directory = "/home/giaupvt/Videos"; }
    else { directory = "/home/giaupvt/Music"; }

    model.loadMediaFiles(directory);

    const auto& mediaFiles = model.getMediaFiles();
    std::cout << "Loaded Media Files:\n";
    if (mediaFiles.empty()) {
        std::cerr << "No supported media files found in the specified directory.\n";
        return -1;
    }

    for (const auto& file : mediaFiles) {
        std::cout << " - " << file << "\n";
    }

    MediaController controller(mediaFiles);
    MediaView view;

    bool exit = false;
    while (!exit) {
        view.displayMenu();
        int choice = view.getUserChoice();
        std::cout << "User selected option: " << choice << "\n"; // Debug user input

        switch (choice) {
        case 1:
            controller.play();
            break;
        case 2:
            controller.pause();
            break;
        case 3:
            controller.resume();
            break;
        case 4:
            controller.stop();
            break;
        case 5:
            controller.playNext();
            break;
        case 6:
            controller.playPrevious();
            break;
        case 7:
            exit = true;
            break;
        default:
            std::cerr << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
