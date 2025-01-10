#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include <vector>
#include <memory>
#include <string>
#include "MediaFile.h"

// MetadataManager (Model)
class MetadataManager {
private:
    std::vector<std::shared_ptr<MediaFile>> listMediaFiles;

public:
    MetadataManager(); // Khai báo hàm khởi tạo mặc định
    ~MetadataManager(); // (Tùy chọn) khai báo hàm hủy

    void addMediaFile(std::shared_ptr<MediaFile> file);
    void removeMediaFile(std::shared_ptr<MediaFile> file);
    void editMediaFile(std::shared_ptr<MediaFile> file);
    void displayAllMediaFiles() const;
};

#endif // METADATA_MANAGER_H

