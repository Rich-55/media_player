#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include <vector>
#include <memory>
#include <string>
#include "MediaFile.h"
#include "../Model/VideoFile.h"

// MetadataManager (Model)
class MetadataManager {
private:
    std::vector<std::shared_ptr<MediaFile>> listMediaFiles;

public:
    MetadataManager(); // Khai báo hàm khởi tạo mặc định
      ~MetadataManager(); // Khai báo destructor

    std::vector<std::shared_ptr<MediaFile>> getAllMediaFile();
    std::vector<std::shared_ptr<MediaFile>> getAllVideoFiles();

    void getData();
    void addMediaFile(std::string, std::string);
    void removeMediaFile(std::shared_ptr<MediaFile>);
    void editMediaFile(std::shared_ptr<MediaFile>);
    
    
};

#endif // METADATA_MANAGER_H

