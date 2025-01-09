// #ifndef MEDIA_SCANNER_CONTROLLER_H
// #define MEDIA_SCANNER_CONTROLLER_H

// #include <string>
// #include <vector>
// #include <filesystem>

// namespace fs = std::filesystem;

// class MediaScannerController {
// private:
//     MetadataManager& metadataManager; 
//     const std::vector<std::string> supportedExtensions = {".mp3", ".mp4", ".jpg", ".png", ".avi", ".mkv"};

//     bool isMediaFile(const fs::path& filePath) const;

// public:
//     MediaScannerController(MetadataManager& manager); 
//     void scanDirectory(const std::string& path);
//     void scanUSB(const std::string& path);
//     void sendScanResultToView(MediaFileView& view);
// };

// #endif
