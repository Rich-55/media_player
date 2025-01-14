#ifndef MEDIA_FILE_MANAGER_CONTROLLER_H
#define MEDIA_FILE_MANAGER_CONTROLLER_H

#include "../Model/MediaFileManager.h"
#include "../View/BaseView.h"
#include "../View/MetadataView.h"
#include "../Controller/MediaScannerController.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>
#include <memory>
#include <unordered_set>
class MediaFileManagerController {
private:
    MediaFileManager& mediaManager;
    std::shared_ptr<BaseView> mediaFileManagerView; 
    std::shared_ptr<MediaScannerController> scannerController;
    
public:
    MediaFileManagerController(MediaFileManager&, std::shared_ptr<BaseView>, std::shared_ptr<MediaScannerController>);

    void handleMediaFileManager();

    void loadData(const std::unordered_set<std::string>&);

    void addDataFile(std::string);
    void addDataFolder(const std::unordered_set<std::string>&);

    void deleteData(std::string);

    std::unordered_set<std::string> getListFileAdded();
    void clearListFileAdded();

    void showAllMediaFile();

    void showAllMediaFileOfVideo();

    void showAllMediaFileOfAudio();

};

#endif 
