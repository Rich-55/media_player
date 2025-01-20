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



    /* Add this part to observer duration */

    controller.addObserver([&]() {
    int duration = controller.getPlayingDuration();
    std::cout << "Current file duration (sec): " << duration << std::endl;
    });






    bool exit = false;
    while (!exit) {
        view.displayMenu();
        int choice = view.getUserChoice();
        std::cout << "User selected option: " << choice << "\n"; // Debug user input

        switch (choice) {
            case 1:
                controller.togglePlayback();
                break;
            case 2:
                controller.playNext();
                break;
            case 3:
                controller.playPrevious();
                break;
            case 4:
                controller.stop();
                break;
            case 5:
                controller.toggleRepeat();
                break;
            case 6:
                exit = true;
                break;
            case 7:
                int volume;
                std::cout << "Enter volume (0-128): ";
                std::cin >> volume;
                controller.setVolume(volume);
                break;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
        }
        // int time = controller.getPlayingDuration();
        // std::cout<< "Duration (sec): " << time << std::endl;
    }
    return 0;
}
