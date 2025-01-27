#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "../Model/ModelManager.h"

#include "../View/ViewManager.h"
#include "../View/ScanView.h"
#include "../View/MediaFileManagerView.h"

#include "MediaScannerController.h"
#include "MediaFileController.h"
#include "MediaFileManagerController.h"
#include "MediaPlaylistManagerController.h"
#include "MediaPlaylistController.h"
#include "PlayerController.h"

#include "../utils/MainMenuException.h"
#include "../utils/ConfigLoader.h"
#include "../utils/UARTSetup.h"


class ControllerManager{
    private:
        ModelManager model;
        ViewManager view;
        std::shared_ptr<UARTManager> uartManager;

        std::shared_ptr<MediaScannerController> scannerController;
        std::shared_ptr<MediaFileController> mediaFileHandlerController;
        std::shared_ptr<MediaPlaylistController> mediaPlaylistController;
        std::shared_ptr<PlayerController> playerController;

        std::unique_ptr<MediaFileManagerController> mediaFileManagerController;
        std::unique_ptr<MediaPlaylistManagerController> mediaPlaylistManagerController;
        
    public:
        ControllerManager(ModelManager model, ViewManager view, std::shared_ptr<UARTManager>  uart);
        
        std::shared_ptr<BaseView> getView(const std::string& viewName);
        
        void runApp();

        bool checkUart();

        void ScanData();

        std::string mediaFileHandler();

        void mediaFileManager();

        void playlistHandler();

        void playlistManager();

        std::string playMusicHandler();

        std::vector<std::string> playPlaylist();

        std::string playVideoHandler();

};

#endif
