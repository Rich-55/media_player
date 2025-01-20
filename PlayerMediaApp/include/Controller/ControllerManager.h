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
#include "../Controller/PlayerController.h"

#include "../utils/MainMenuException.h"
#include "../utils/ConfigLoader.h"
#include <limits>

class ControllerManager{
    private:
        ModelManager model;
        ViewManager view;
        std::shared_ptr<MediaScannerController> scannerController;
        std::shared_ptr<MediaFileController> mediaFileHandlerController;
        std::shared_ptr<MediaPlaylistController> mediaPlaylistController;
        
        std::unique_ptr<MediaFileManagerController> mediaFileManagerController;
        std::unique_ptr<MediaPlaylistManagerController> mediaPlaylistManagerController;
        std::unique_ptr<PlayerController> playerController;
    public:
        ControllerManager(ModelManager, ViewManager);
        
        std::shared_ptr<BaseView> getView(const std::string& viewName);
        bool getYesNoInput(const std::string& prompt);

        void runApp();

        void ScanData();

        void mediaFileManager();

        void metadataFileHandler();

        void playlistManager();

        void playlistHandler();

        std::string playMusicHandler();

        std::vector<std::string> playPlaylist();

};

#endif
