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
        std::shared_ptr<PlayerController> playerController;

        std::unique_ptr<MediaFileManagerController> mediaFileManagerController;
        std::unique_ptr<MediaPlaylistManagerController> mediaPlaylistManagerController;
        asio::io_context io_context;                      // Quản lý IO cho UART
        std::unique_ptr<asio::serial_port> serial_port;   // Serial port cho UART
        std::thread uartThread;    
    public:
        ControllerManager(ModelManager, ViewManager);
        
        std::shared_ptr<BaseView> getView(const std::string& viewName);
        
        void setUpUART(const std::string& port, unsigned int baud_rate);
        void stopUART();

        void runApp();

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
