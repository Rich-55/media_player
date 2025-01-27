#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../Model/MediaFileManager.h"
#include "../Model/FolderManager.h"
#include "../View/BaseView.h"
#include "../View/ScanView.h"
#include "../utils/ScanException.h"
#include "../utils/ConfigLoader.h"

class MediaScannerController{
    private:
        
        MediaFileManager& mediaFileManager;
        PlaylistManager& playlistManager;
        FolderManager& folderManager;
        std::shared_ptr<BaseView> scanView;
        std::unordered_set<std::string> listPaths;
        std::unordered_set<std::string> listPathsAdded;
    public:
        
        MediaScannerController(MediaFileManager& mediaFileManager, PlaylistManager& playlistManager, FolderManager& folderManager,std::shared_ptr<BaseView> scanView);

        void handleScan(bool isRunning);

        void addDataFileWithFolder(std::string nameFolder, std::string nameLocation);

        void loadData();
        void loadFolder();
        void loadMediaPlaylist();

        std::unordered_set<std::string> getListPaths();

        void scanUSBDevices();
        void scanHomeDirectory();

        bool checkFolderDirectory();
        bool checkFolderUSB();

        std::unordered_set<std::string> scanFolder(const std::string &path);

};

#endif
