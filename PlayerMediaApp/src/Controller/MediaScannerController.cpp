// #include "../../include/Controller/MediaScannerController.h"

// // #include "MediaScannerController.h"
// // #include "MetadataManager.h"
// // #include "MediaFileView.h"
// // #include <iostream>
// // #include <algorithm>

// MediaScannerController::MediaScannerController(MetadataManager& manager) : metadataManager(manager) {}

// bool MediaScannerController::isMediaFile(const fs::path& filePath) const {
//     return std::find(supportedExtensions.begin(), supportedExtensions.end(), filePath.extension().string()) != supportedExtensions.end();
// }

// void MediaScannerController::scanDirectory(const std::string& path) {
//     std::vector<std::string> mediaFiles; 
//     try {
//         if (!fs::exists(path) || !fs::is_directory(path)) {
//             throw std::runtime_error("Invalid path or not a directory.");
//         }

//         for (const auto& entry : fs::recursive_directory_iterator(path)) {
//             if (entry.is_regular_file() && isMediaFile(entry.path())) {
//                 mediaFiles.push_back(entry.path().string());
//             }
//         }
        
//         // Process metadata for each found media file
//         for (const auto& file : mediaFiles) {
//             metadataManager.processMetadata(file); 
//         }

//     } catch (const std::exception& e) {
//         std::cerr << "Error while scanning directory: " << e.what() << std::endl;
//     }
// }

// void MediaScannerController::scanUSB(const std::string& path) {
//     // Assuming USB scanning logic is similar to directory scanning for this example.
//     // In a real application, you might need to handle USB device detection, mounting, etc.
//     scanDirectory(path); 
// }

// void MediaScannerController::sendScanResultToView(MediaFileView& view) {
//     // Assuming MetadataManager has a method to get the processed metadata
//     std::vector<MediaFileInfo> processedFiles = metadataManager.getProcessedFiles();

//     // Send the metadata to the view for display
//     view.displayMediaFiles(processedFiles);
// }
