#include "../../include/Controller/MediaFileController.h"


MediaFileController::MediaFileController(MediaFile* model, MediaFileView& view) 
    : model(model), view(view) {}

void MediaFileController::updateView() {
    view.displayFileDetails(*model);
}

void MediaFileController::setTrackName(const std::string& newTrackName) {
    // Assuming you have a dynamic_cast or similar mechanism to handle specific file types:
    if (AudioFile* audioFile = dynamic_cast<AudioFile*>(this->model)) {
        audioFile->setTrackName(newTrackName);
    }
}

void MediaFileController::setAlbum(const std::string& newAlbum) {
    if (AudioFile* audioFile = dynamic_cast<AudioFile*>(this->model)) {
        audioFile->setAlbum(newAlbum);
    }
}

void MediaFileController::setArtist(const std::string& newArtist) {
    if (AudioFile* audioFile = dynamic_cast<AudioFile*>(this->model)) {
        audioFile->setArtist(newArtist);
    }
}

void MediaFileController::setGenre(const std::string& newGenre) {
    if (AudioFile* audioFile = dynamic_cast<AudioFile*>(this->model)) {
        audioFile->setGenre(newGenre);
    }
}

void MediaFileController::saveChanges() {
    if (AudioFile* audioFile = dynamic_cast<AudioFile*>(this->model)) {
        audioFile->editMediaFile();
    }
}
// void MediaFileController::handleMenuInput(int choice) {
//     switch (choice) {
//         case 1: {
//             if (!audioFiles.empty()) {
//                 view.showMetadataFile(*audioFiles[0]); // Pass the first AudioFile object to the view
//             } else {
//                 std::cout << "No audio files loaded." << std::endl;
//             }
//             break;
//         }
//         case 2: {
//             if (!audioFiles.empty()) {
//                 audioFiles[0]->editMediaFile(); // Call editMediaFile on the first AudioFile object
//             } else {
//                 std::cout << "No audio files loaded." << std::endl;
//             }
//             break;
//         }
//         case 0:
//             std::cout << "Exiting program!" << std::endl;
//             break;
//         default:
//             std::cout << "Invalid choice. Please try again." << std::endl;
//             break;
//     }
// }

// void MediaFileController::addMediaFileFromUSB(const std::shared_ptr<MediaFile> &file) {
//     std::cout << "Adding media file from USB: " << file->getName() << std::endl;
//     metadataManager.addMediaFile(file);
// }

// void MediaFileController::addMediaFileFromPC(const std::shared_ptr<MediaFile> &file) {
//     std::cout << "Adding media file from PC: " << file->getName() << std::endl;
//     metadataManager.addMediaFile(file);
// }

// void MediaFileController::editMediaFile(const std::shared_ptr<MediaFile> &file) {
//     std::cout << "Editing media file: " << file->getName() << std::endl;
//     file->editMediaFile();
// }

// void MediaFileController::removeMediaFile(const std::shared_ptr<MediaFile> &file) {
//     const auto &files = metadataManager.getMediaFiles();
//     auto it = std::find(files.begin(), files.end(), file);

//     if (it != files.end()) {
//         std::cout << "Removing media file: " << file->getName() << std::endl;
//         metadataManager.getMediaFiles().erase(it);
//     } else {
//         std::cout << "Media file not found: " << file->getName() << std::endl;
//     }
// }

// void MediaFileController::displayAllMediaFiles(MediaFileView &view) {
//     view.showAllMediaFile(metadataManager);
// }

// void MediaFileController::sendMediaFileToView(MediaFileView &view, const std::shared_ptr<MediaFile> &file) {
//     view.showMetadataFile(*file);
// }
