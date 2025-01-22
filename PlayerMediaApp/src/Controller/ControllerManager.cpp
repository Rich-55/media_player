#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, ViewManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaPlaylistController(nullptr), playerController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistManagerController(nullptr) 
{

}

std::shared_ptr<BaseView> ControllerManager::getView(const std::string& viewName) {
    auto viewPtr = view.getView(viewName);
    if (!viewPtr) {
        throw std::runtime_error("Error: " + viewName + " not found or failed to initialize.");
    }
    return viewPtr;
}


// ScanData function: Scan data from the folder in directory or USB
void ControllerManager::ScanData() {
    try {
        auto scanView = getView("ScanView");

        if (!scannerController) {
            scannerController = std::make_shared<MediaScannerController>(
                model.getMediaFileManager(), 
                model.getPlaylistManager(),
                model.getFolderManager(), 
                scanView
            );
        }
        scannerController->handleScan(false);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Media File Handler function: Handle metadata file (add, delete, edit)
std::string ControllerManager::mediaFileHandler() {
    try {
        std::string fileName;
        auto mediaFileHandlerView = getView("MediaFileHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");

        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                mediaFileHandlerView,
                scannerController
            );
        } 
        
        fileName = mediaFileManagerController->showAllMediaFile();
        std::cout << "File name: " << fileName << std::endl;
        if(fileName == "exit"){
            return "exit";
        }

        if (PlayerController::currentPlayingFile == model.getMediaFileManager().getMediaFile(fileName)->getPath()) {
            return "File is running, can't edit";
        }


        mediaFileHandlerController = std::make_shared<MediaFileController>(
            model.getMediaFile(fileName), 
            mediaFileHandlerView
        );

        mediaFileManagerController->addMediaFileController(fileName, mediaFileHandlerController);
        
        mediaFileHandlerController->handlerMediaFile();
    } catch (const std::exception& e) {
        return e.what();
    }
    return "";
}

// MediaFileManager function: Handle media file (add, delete, edit, view)
void ControllerManager::mediaFileManager() {
    try {
        auto scanView = getView("ScanView");
        
        if (!scannerController) { 
            scannerController = std::make_shared<MediaScannerController>(
                model.getMediaFileManager(), 
                model.getPlaylistManager(),
                model.getFolderManager(), 
                scanView
            );
        }

        auto mediaFileManagerView = getView("MediaFileManagerView");
        auto mediaFileHandlerView = getView("MediaFileHandlerView");

        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                mediaFileHandlerView,
                scannerController
            );
        }
        mediaFileManagerController->handleMediaFileManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlaylistHandler function: Handle playlist (add media, delete media, view)
void ControllerManager::playlistHandler() {
    try {
        std::string playlistName;

        auto playlistManagerView = getView("PlaylistManagerView");
        auto playlistHandlerView = getView("PlaylistHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");

        if(!mediaPlaylistManagerController){
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
                model.getPlaylistManager(), 
                model.getMediaFileManager(),
                model.getFolderManager(),
                mediaFileManagerView,
                playlistManagerView,
                playlistHandlerView
            );
        }

        if (!mediaFileManagerController) {
            auto scanView = getView("ScanView");
        
            if (!scannerController) { 
                scannerController = std::make_shared<MediaScannerController>(
                    model.getMediaFileManager(), 
                    model.getPlaylistManager(),
                    model.getFolderManager(), 
                    scanView
                );
            }

            auto mediaFileManagerView = getView("MediaFileManagerView");
            auto mediaFileHandlerView = getView("MediaFileHandlerView");
            
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(
                model.getMediaFileManager(), 
                mediaFileManagerView, 
                mediaFileHandlerView,
                scannerController
            );
        }

        if (!model.getPlaylistManager().checkPlaylist()) {
            std::string name;
            name = playlistManagerView->showMenuCreatePlaylist();
            if(name == "exit"){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        playlistName = mediaPlaylistManagerController->displayAllPlaylist();

        if (playlistName == "exit") {
            return;
        }

        mediaPlaylistController = std::make_shared<MediaPlaylistController>(
            model.getMediaFileManager(), 
            model.getFolderManager(),
            model.getPlaylist(playlistName), 
            mediaFileManagerView,
            playlistHandlerView
        );

        mediaPlaylistManagerController->addMediaPlaylistController(playlistName, mediaPlaylistController);
        
        mediaPlaylistController->handlerPlaylist();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlaylistManager function: Handle playlist (create, delete, view)
void ControllerManager::playlistManager() {
    try {
        auto playlistManagerView = getView("PlaylistManagerView");
        auto playlistHandlerView = getView("PlaylistHandlerView");
        auto mediaFileManagerView = getView("MediaFileManagerView");

        if(!mediaPlaylistManagerController){
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
                model.getPlaylistManager(), 
                model.getMediaFileManager(),
                model.getFolderManager(),
                mediaFileManagerView,
                playlistManagerView,
                playlistHandlerView
            );
        }
        
        if (!model.getPlaylistManager().checkPlaylist()) {
            std::string name;
            name = playlistManagerView->showMenuCreatePlaylist();
            if(name == "exit"){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        mediaPlaylistManagerController->handlerPlaylistManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// PlayMusicHandler function: Play single music
std::string ControllerManager::playMusicHandler() {
    try {
        auto mediaFileManagerView = getView("MediaFileManagerView");
        std::string fileName;

        fileName = mediaFileManagerView->displayAllMediaFileOfAudio(model.getMediaFileManager());

        if (fileName == "exit") {
            return "exit";
        }

        std::string filePath = model.getMediaFileManager().getMediaFile(fileName)->getPath();

        return filePath;
    } catch (const std::exception& e) {
        return e.what();
    }
}

// PlayPlaylist function: Play playlist
std::vector<std::string> ControllerManager::playPlaylist() {
    std::string playlistName;
    auto playlistManagerView = getView("PlaylistManagerView");
    auto playlistHandlerView = getView("PlaylistHandlerView");
    auto mediaFileManagerView = getView("MediaFileManagerView");

    if(!mediaPlaylistManagerController){
        mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(
            model.getPlaylistManager(), 
            model.getMediaFileManager(),
            model.getFolderManager(),
            mediaFileManagerView,
            playlistManagerView,
            playlistHandlerView
        );
    }
    
    if (!model.getPlaylistManager().checkPlaylist()) {
        std::string name;
        name = playlistManagerView->showMenuCreatePlaylist();
        if(name == "exit"){
            return {"exit"};
        }
        mediaPlaylistManagerController->createPlaylist(name);
    }

    playlistName = mediaPlaylistManagerController->displayAllPlaylist();

    if (playlistName == "exit") {
        return {"exit"};
    }

    mediaPlaylistController = std::make_shared<MediaPlaylistController>(
        model.getMediaFileManager(), 
        model.getFolderManager(),
        model.getPlaylist(playlistName), 
        mediaFileManagerView,
        playlistHandlerView
    );

    
    return mediaPlaylistController->getListPathMediaFiles();
}

std::string ControllerManager::playVideoHandler() {
    try {
        auto mediaFileManagerView = getView("MediaFileManagerView");
        std::string fileName;

        fileName = mediaFileManagerView->displayAllMediaFileOfVideo(model.getMediaFileManager());

        if (fileName.empty()) {
            return "exit";
        }

        std::string filePath = model.getMediaFileManager().getMediaFile(fileName)->getPath();

        return filePath;
    } catch (const std::exception& e) {
        return e.what();
    }
}

void asyncHandleUart(asio::serial_port& serial, std::shared_ptr<PlayerController>& playerController) {
    auto buffer = std::make_shared<asio::streambuf>();

    asio::async_read_until(serial, *buffer, '\n',
        [buffer, &serial, &playerController](const boost::system::error_code& error, std::size_t bytes_transferred) {
            (void)bytes_transferred; 
            if (!error) {
                std::istream input_stream(buffer.get());
                std::string received_data;
                std::getline(input_stream, received_data);

                if (playerController) {
                    if (received_data == "play") {
                        playerController->togglePlayback();
                    } else if (received_data == "pause") {
                        playerController->togglePlayback();
                    } else if (received_data == "SW3") {
                        playerController->stop();
                    } else if (received_data == "next") {
                        playerController->playNext();
                    } else if (received_data == "previous") {
                        playerController->playPrevious();
                    } else {
                        try {
                            int volume = std::stoi(received_data);
                            std::cout << "Volume: " << volume << std::endl;
                            playerController->setVolume(volume);
                        } catch (...) {
                        }
                    }
                } else {
                }

                asyncHandleUart(serial, playerController);
            } else {
                std::cerr << "UART read error: " << error.message() << std::endl;
            }
        }
    );
}

void ControllerManager::stopUART() {
    try {
        if (serial_port) {
            serial_port->close();
        }
        io_context.stop();
        if (uartThread.joinable()) {
            uartThread.join();
        }
        std::cout << "UART stopped successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error stopping UART: " << e.what() << std::endl;
    }
}

void ControllerManager::setUpUART(const std::string& port, unsigned int baud_rate) {
    try {
        // Khởi tạo serial_port
        serial_port = std::make_unique<asio::serial_port>(io_context, port);
        serial_port->set_option(asio::serial_port_base::baud_rate(baud_rate));
        serial_port->set_option(asio::serial_port_base::character_size(8));
        serial_port->set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
        serial_port->set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        serial_port->set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

        // Đăng ký xử lý UART
        asyncHandleUart(*serial_port, playerController);

        // Chạy io_context trên một luồng riêng
        uartThread = std::thread([this]() {
            try {
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "UART thread error: " << e.what() << std::endl;
            }
        });

        std::cout << "UART setup completed on port " << port << " with baud rate " << baud_rate << ".\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to set up UART: " << e.what() << std::endl;
    }
}



void ControllerManager::runApp() {
    if (model.getFolderManager().getListFolderDirectory().empty() &&
        model.getFolderManager().getListFolderUSB().empty()) {
        return;
    }
    setUpUART("/dev/ttyACM1", 115200);

    auto mainMenuView = getView("MainMenuView");
    std::string typePlay = "noplay"; 
    std::string media;
    std::vector<std::string> listMedia;
    int choice;
    std::string error;

    while (true) {
        try {
            // Hiển thị menu chính
            choice = mainMenuView->showMenuWithPlayer(
                model.getMediaFileManager(),
                playerController,
                typePlay,
                error
            );

            switch (choice) {
                case METADATA_FILE_HANDLER: {
                    std::string checkError = mediaFileHandler();
                    if (checkError == "exit") {
                        break;
                    }
                    if (checkError == "File is running, can't edit") {
                        error = checkError;
                    }
                    break;
                }
                case MEDIA_FILE_MANAGER:
                    mediaFileManager();
                    break;
                case PLAYLIST_MANAGER:
                    playlistManager();
                    break;
                case PLAYLIST_HANDLER:
                    playlistHandler();
                    break;
                case PLAY_MUSIC: {
                    media = playMusicHandler(); 
                    if (media == "exit") {
                        break;
                    }
                    if (!media.empty()) {
                        typePlay = "single"; 
                        std::vector<std::string> singleMedia = {media};
                        if (playerController) {
                            playerController->stop();
                            playerController = nullptr;
                        }
                        playerController = std::make_shared<PlayerController>(singleMedia); 
                        playerController->play();
                    }
                    break;
                }
                case PLAY_PLAYLIST: {
                    listMedia = playPlaylist();
                    if (listMedia.size() == 1 && listMedia[0] == "exit") {
                        break;
                    }
                    if (!listMedia.empty()) {
                        typePlay = "playlist"; 
                        if (playerController) {
                            playerController->stop();
                            playerController = nullptr;
                        }
                        playerController = std::make_shared<PlayerController>(listMedia); 
                        playerController->play();
                    } else {
                        error = "No media files to play.";
                    }
                    break;
                }
                case PLAY_VIDEO: {
                    media = playVideoHandler(); 
                    if (media == "exit") {
                        break;
                    }
                    if (!media.empty()) {
                        typePlay = "single"; 
                        std::vector<std::string> singleMedia = {media};
                        if (playerController) {
                            playerController->stop();
                            playerController = nullptr;
                        }
                        playerController = std::make_shared<PlayerController>(singleMedia); 
                        playerController->play();
                    }
                    break;
                }
                case EXIT_MAIN_MENU:
                    if(playerController){
                        playerController->stop();
                        playerController = nullptr;
                    }
                    stopUART();
                    return; 
                default:
                    throw InvalidChoiceException();
            }
        } catch (const std::exception& e) {
            error = e.what();
        }
    }
}
