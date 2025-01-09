// #include "../../include/Model/MetadataManager.h"

// void MetadataManager::addMediaFileFromUSB(std::shared_ptr<MediaFile> file) {
//     listMediaFiles.push_back(file);
//     std::cout << "Added media file from USB: " << file->filename << std::endl;
// }

// void addMediaFileFromPC(std::shared_ptr<MediaFile> file) {
//         listMediaFiles.push_back(file);
//         std::cout << "Added media file from PC: " << file->filename << std::endl;
// }

// void editMediaFile(std::shared_ptr<MediaFile> file) {
//     auto it = std::find_if(listMediaFiles.begin(), listMediaFiles.end(),
//         [&](const std::shared_ptr<MediaFile>& f) {
//             return f->filename == file->filename;
//         });

//     if (it != listMediaFiles.end()) {
//         (*it)->filename = file->filename; 
//         (*it)->filepath = file->filepath;
//         std::cout << "Edited media file: " << (*it)->filename << std::endl;
//     } else {
//         std::cout << "Media file not found for editing." << std::endl;
//     }
// }

// void removeMediaFile(std::shared_ptr<MediaFile> file) {
//     auto it = std::remove_if(listMediaFiles.begin(), listMediaFiles.end(),
//         [&](const std::shared_ptr<MediaFile>& f) {
//             return f->filename == file->filename;
//         });

//     if (it != listMediaFiles.end()) {
//         listMediaFiles.erase(it, listMediaFiles.end());
//         std::cout << "Removed media file: " << file->filename << std::endl;
//     } else {
//         std::cout << "Media file not found for removal." << std::endl;
//     }
// }

// void getAllMediaFiles() const {
//     std::cout << "--- All Media Files ---" << std::endl;
//     if (listMediaFiles.empty()) {
//         std::cout << "No media files found." << std::endl;
//     } else {
//         for (const auto& file : listMediaFiles) {
//             file.getName();
//             std::cout << "------------------------" << std::endl;
//         }
//     }
// }
