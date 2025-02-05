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
        virtual ~MediaScannerController() = default;
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

        virtual std::unordered_set<std::string> scanFolder(const std::string &path);

        virtual bool has_extension(const std::string &filename, const std::string &extension);
        virtual std::vector<std::string> list_folders(const std::string &path);
        virtual std::vector<std::string> list_media_files(const std::string &path);
        virtual std::vector<std::string> scan_all_folders(const std::string &path);

        virtual bool fileExists(const std::string& path);

};

#endif
