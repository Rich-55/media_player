#include "include/Model/MediaFile.h"
#include "include/Model/VideoFile.h"
#include "include/View/MediaFileView.h"
#include "include/Controller/MediaFileController.h"
#include "include/Model/MetadataManager.h"


// Main function
int main() {
    MetadataManager model;
    MediaFileView view;
    MediaFileController controller(model, view);

    int choice;
    std::shared_ptr<MediaFile> videoFile;
    std::string filePath;

    do {
        std::cout << "\nMedia File Management Menu:\n";
        std::cout << "1. Add Video File\n";
        std::cout << "2. Edit Video File\n";
        std::cout << "3. Remove Video File\n";
        std::cout << "4. Display All Media Files\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter video file path: ";
                std::cin >> filePath;
                videoFile = std::make_shared<VideoFile>();
                videoFile->inputMediaFile(filePath);
                controller.addMediaFile(videoFile);
                break;
            case 2:
                controller.updateView();
                std::cout << "Enter the file name of the video to edit: ";
                std::cin >> filePath;
                videoFile = std::make_shared<VideoFile>();
                videoFile->inputMediaFile(filePath);
                controller.editMediaFile(videoFile);
                break;
            case 3:
                controller.updateView();
                std::cout << "Enter the file name of the video to remove: ";
                std::cin >> filePath;
                videoFile = std::make_shared<VideoFile>();
                videoFile->inputMediaFile(filePath);
                controller.removeMediaFile(videoFile);
                break;
            case 4:
                controller.updateView();
                break;
            case 0:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice! Try again.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
