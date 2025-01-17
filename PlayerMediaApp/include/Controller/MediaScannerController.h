#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../View/ScanView.h"
#include "../Model/MediaFileManager.h"
#include "../Model/FolerManager.h"
#include "../View/BaseView.h"

#include "../utils/ScanException.h"
#include "../utils/ConfigLoader.h"

#include <memory>
#include <unordered_set>
#include <unistd.h>


class MediaScannerController{
    private:
        MediaFileManager& mediaFileManager;
        PlaylistManager& playlistManager;
        FolderManager& folderManager;
        std::shared_ptr<BaseView> scanView;
        std::unordered_set<std::string> listPaths;
        std::unordered_set<std::string> listPathsAdded;
    public:
        
        MediaScannerController(MediaFileManager&, PlaylistManager&,FolderManager&, std::shared_ptr<BaseView>);

        void handleScan(bool);

        void addDataFileWithFolder(std::string, std::string);

        void loadData();
        void loadMediaPlaylist();

        std::unordered_set<std::string> getListPaths();

        void scanUSBDevices();
        void scanHomeDirectory();

        bool checkFolderDirectory();
        bool checkFolderUSB();

        std::unordered_set<std::string> scanFolder(const std::string &path);

        
};

#endif
