#include "include/Model/MediaFile.h"
#include "include/Model/VideoFile.h"
#include "include/View/MediaFileView.h"
#include "include/Controller/MediaFileController.h"


int main() {
    std::string filePath = "video/take.mp4"; // Path to the MP4 file

    MediaFile *video = new VideoFile;
    video->inputMediaFile(filePath.c_str());

    // Create view and controller
    MediaFileView view;
    MediaFileController controller(video, view);

    // Update view
    controller.updateView();

    // Clean up
    delete video;
    return 0;
}