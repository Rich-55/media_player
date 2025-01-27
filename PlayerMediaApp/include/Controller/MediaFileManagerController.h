#ifndef MEDIA_FILE_MANAGER_CONTROLLER_H
#define MEDIA_FILE_MANAGER_CONTROLLER_H

#include "../Model/MediaFileManager.h"
#include "../View/BaseView.h"
#include "MediaScannerController.h"
#include "MediaFileController.h"

#include "../utils/MediaFileManagerException.h"
#include "../utils/ConfigLoader.h"

class MediaFileManagerController {
private:
    MediaFileManager& mediaManager;
    std::shared_ptr<BaseView> mediaFileManagerView; 
    std::shared_ptr<BaseView> mediaFileHandlerView;
    std::shared_ptr<MediaScannerController> scannerController;
    std::unordered_map<std::string, std::shared_ptr<MediaFileController>> listMediaFileController;
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
    MediaFileManagerController(MediaFileManager& mediaManager, std::shared_ptr<BaseView> mediaManagerView, std::shared_ptr<BaseView> mediaFileView,std::shared_ptr<MediaScannerController> c);

    void handleMediaFileManager();

    void addMediaFileController(std::string fileName, std::shared_ptr<MediaFileController> mediaFileController);

    std::shared_ptr<MediaFileController> getMediaFileController(const std::string& fileName);

    int addDataFile(std::string pathName);
    void addDataFolder(const std::unordered_set<std::string> &listPathName);

    bool deleteData(std::string fileName);

    std::string showAllMediaFile();

    std::string showAllMediaFileOfVideo();

    std::string showAllMediaFileOfAudio();

};

#endif 
