#include "include/Model/MediaFile.h"
#include "include/Model/VideoFile.h"
#include "include/View/MediaFileView.h"
#include "include/Controller/MediaFileController.h"

#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>

int main() {
    std::string filePath = "video/take.mp3";
    TagLib::MPEG::File file(filePath.c_str());

    if (!file.isOpen()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    if (file.save()) {
        std::cout << "Metadata updated successfully!" << std::endl;
    } else {
        std::cerr << "Failed to save metadata." << std::endl;
    }

    // Create objects
    VideoFile* video = new VideoFile("example.mp4", "/videos", 102.5, "3600", "Video", "H.264", 5000, "1920x1080");

    // Create view and controller
    MediaFileView view;
    MediaFileController controller(video, view);

    controller.updateView();

    delete video;

    return 0;
}