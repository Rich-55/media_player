#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "../Model/ModelManager.h"

#include "../View/ViewManager.h"
#include "../View/ScanView.h"
#include "../View/MediaFileManagerView.h"

#include "../Controller/MediaScannerController.h"
#include "../Controller/MediaFileController.h"
#include "../Controller/MediaFileManagerController.h"
#include "../Controller/MediaPlaylistManagerController.h"
#include "../Controller/MediaPlaylistController.h"

#include "../utils/MainMenuException.h"
#include "../utils/ConfigLoader.h"
#include <limits>

class ControllerManager{
    private:
        ModelManager model;
        ViewManager view;
        std::shared_ptr<MediaScannerController> scannerController;
        std::unique_ptr<MediaFileController> mediaFileHandlerController;
        std::unique_ptr<MediaFileManagerController> mediaFileManagerController;
        std::unique_ptr<MediaPlaylistController> mediaPlaylistController;
        std::unique_ptr<MediaPlaylistManagerController> mediaPlaylistManagerController;

    public:
        ControllerManager(ModelManager, ViewManager);
        
        std::shared_ptr<BaseView> getView(const std::string&);
        bool getYesNoInput(const std::string&);

        void runApp();

        void ScanData();

        void mediaFileManager();

        void metadataFileHandler();

        void playlistManager();

        void playlistHandler();

};

#endif
