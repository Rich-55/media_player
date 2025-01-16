#ifndef MEDIA_FILE_MANAGER_CONTROLLER_H
#define MEDIA_FILE_MANAGER_CONTROLLER_H

#include "../Model/MediaFileManager.h"
#include "../View/BaseView.h"
#include "../View/MetadataView.h"
#include "../Controller/MediaScannerController.h"

#include "../utils/MediaFileManagerException.h"
#include "../utils/ConfigLoader.h"

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
protected:
    bool isValidPath(const std::string& path) {
        if (path.empty()) {
            return false;
        }

        if (path.find('/') == std::string::npos && path.find('\\') == std::string::npos) {
            return false;
        }

        return true; 
    }

    bool fileExists(const std::string& path) {
        return access(path.c_str(), F_OK) == 0; 
    }

public:
    MediaFileManagerController(MediaFileManager&, std::shared_ptr<BaseView>, std::shared_ptr<MediaScannerController>);

    void handleMediaFileManager();

    void loadData(const std::unordered_set<std::string>&);

    int addDataFile(std::string);
    void addDataFolder(const std::unordered_set<std::string>&);

    bool deleteData(std::string);

    std::unordered_set<std::string> getListFileAdded();
    void clearListFileAdded();

    std::string showAllMediaFile();

    void showAllMediaFileOfVideo();

    void showAllMediaFileOfAudio();

};

#endif 
