#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "../Model/ModelManager.h"

#include "../View/CLIManager.h"
#include "../View/ScanView.h"
#include "../View/MediaFileManagerView.h"

#include "../Controller/MediaScannerController.h"
#include "../Controller/MediaFileController.h"
#include "../Controller/MediaFileManagerController.h"
#include "../Controller/MediaPlaylistManagerController.h"
#include "../Controller/MediaPlaylistController.h"
#define ScanHomeDirectory 1

class ControllerManager{
    private:
        ModelManager model;
        CLIManager view;
        std::unique_ptr<MediaScannerController> scannerController;
        std::unique_ptr<MediaFileController> mediaFileHandlerController;
        std::unique_ptr<MediaFileManagerController> mediaFileManagerController;
        std::unique_ptr<MediaPlaylistController> mediaPlaylistController;
        std::unique_ptr<MediaPlaylistManagerController> mediaPlaylistManagerController;

    public:
        ControllerManager(ModelManager, CLIManager);
        
        void ScanData();

        void mediaFileManager();

        void metadataFileHandler();

        void playlistManager();

        void playlistHandler();

};

#endif
