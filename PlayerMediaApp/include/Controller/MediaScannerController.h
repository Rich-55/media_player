#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../View/ScanView.h"
#include "../Model/MediaFileManager.h"
#include "../Model/FolerManager.h"
#include "../View/BaseView.h"
#include <memory>
#include <unordered_set>
#define ScanHomeDirectory 1
#define ScanUSBDevices 2
#define Exit 0

class MediaScannerController{
    private:
        MediaFileManager& mediaFileManager;
        FolderManager& folderManager;
        std::shared_ptr<BaseView> scanView;
        std::unordered_set<std::string> listPaths;
        std::unordered_set<std::string> listPathsAdded;
    public:
        MediaScannerController(MediaFileManager&, FolderManager&, std::shared_ptr<BaseView>);

        void handleScan();

        void addDataFileWithFolder(std::string, std::string);

        void loadData();

        std::unordered_set<std::string> getListPaths();

        void scanUSBDevices();
        void scanHomeDirectory();

        bool checkFolderDirectory();
        bool checkFolderUSB();

        std::unordered_set<std::string> scanFolder(const std::string &path);

        



};

#endif
