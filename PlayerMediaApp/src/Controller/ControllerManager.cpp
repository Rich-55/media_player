#include "../../include/Controller/ControllerManager.h"

std::thread musicThread;

std::atomic<bool> isMusicRunning{false};
std::atomic<bool> stopMusic{false};
std::atomic<bool> ControllerManager::isPaused{false};

std::string fileName = "";

ControllerManager::ControllerManager(ModelManager m, ViewManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaPlaylistController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistManagerController(nullptr) 
{
    if (isMusicRunning) {
        std::cout << "Stopping music before exiting..." << std::endl;
        stopMusic = true;

        if (musicThread.joinable()) {
            musicThread.join();
        }

        isMusicRunning = false;
    }
}

std::shared_ptr<BaseView> ControllerManager::getView(const std::string& viewName) {
    auto viewPtr = view.getView(viewName);
    if (!viewPtr) {
        throw std::runtime_error("Error: " + viewName + " not found or failed to initialize.");
    }
    return viewPtr;
}

bool ControllerManager::getYesNoInput(const std::string& prompt) {
    std::string choice;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please enter Y/y or N/n.\n";
            continue;
        }

        if (choice == "Y" || choice == "y") return true;
        if (choice == "N" || choice == "n") return false;

        std::cerr << "Invalid input. Please enter Y/y or N/n.\n";
    }
}

// ScanData function
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

// MetadataFileHandler function
void ControllerManager::metadataFileHandler() {
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
        if(fileName == ""){
            return;
        }
        
        mediaFileHandlerController = std::make_shared<MediaFileController>(
            model.getMediaFile(fileName), 
            mediaFileHandlerView
        );

        mediaFileManagerController->addMediaFileController(fileName, mediaFileHandlerController);
        
        mediaFileHandlerController->handlerMediaFile();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// MediaFileManager function
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

// PlaylistHandler function
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
            if (!getYesNoInput("Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ")) {
                return;
            }
            std::string name;
            std::cout << "Enter the name of the playlist (type '0' to cancel): ";
            std::getline(std::cin, name); 
            if(name == "0"){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        playlistName = mediaPlaylistManagerController->displayAllPlaylist();

        if (playlistName == "") {
            std::cout << "Returning to the main menu.\n";
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

// PlaylistManager function
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
            if (!getYesNoInput("No playlists found. Do you want to create a new playlist? (Y/N): ")) {
                return;
            }
            std::string name;
            std::cout << "Enter the name of the playlist (type '0' to cancel): ";
            std::getline(std::cin, name); 
            if(name == "0"){
                return;
            }
            mediaPlaylistManagerController->createPlaylist(name);
        }

        mediaPlaylistManagerController->handlerPlaylistManager();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void playMusic(const std::string& filePath, std::atomic<bool>& stopMusic, std::atomic<bool>& isPaused) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_Mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "Failed to load music file: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    std::cout << "Playing music: " << filePath << std::endl;

    // Vòng lặp phát nhạc
    while (!stopMusic) {
        if (isPaused) {
            Mix_PauseMusic();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            Mix_ResumeMusic();
            SDL_Delay(100); // Giảm tải CPU
        }
    }

    // Dừng và giải phóng tài nguyên
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    std::cout << "Music playback stopped." << std::endl;
}

void ControllerManager::playMusicHandler() {
    if (isMusicRunning) {
        std::cout << "Stopping currently playing music..." << std::endl;
        stopMusic = true;

        if (musicThread.joinable()) {
            musicThread.join(); // Đợi thread phát nhạc dừng
        }

        isMusicRunning = false;
        stopMusic = false; // Đặt lại trạng thái
        std::cout << "Music stopped." << std::endl;
    }

    // Chọn bài hát mới để phát
    auto mediaFileManagerView = getView("MediaFileManagerView");
    fileName = mediaFileManagerView->displayAllMediaFileOfAudio(model.getMediaFileManager());

    if (fileName == "") {
        return;
    }

    std::string filePath = model.getMediaFileManager().getMediaFile(fileName)->getPath();

    // Bắt đầu phát nhạc trong thread mới
    isMusicRunning = true;
    musicThread = std::thread([this, filePath]() {
        playMusic(filePath, stopMusic, isPaused); // Truyền isPaused vào playMusic
        isMusicRunning = false; // Đặt lại trạng thái khi nhạc dừng
    });
}

void ControllerManager::runApp() {
    auto mainMenuView = getView("MainMenuView");
    while (true) {
        try {
            int choice = mainMenuView->showMenuWithPlayer(model.getMediaFileManager(), fileName);
            switch (choice) {
                case METADATA_FILE_HANDLER:
                    metadataFileHandler();
                    break;
                case MEDIA_FILE_MANAGER:
                    mediaFileManager();
                    break;
                case PLAYLIST_MANAGER:
                    playlistManager();
                    break;
                case PLAYLIST_HANDLER:
                    playlistHandler();
                    break;
                case PLAY_MUSIC:
                    playMusicHandler();
                    break;
                case -2: // Pause
                    if (isMusicRunning && !isPaused) {
                        std::cout << "Pausing music..." << std::endl;
                        isPaused = true;
                    } else {
                        std::cout << "No music to pause or already paused." << std::endl;
                    }
                    break;
                case -3: // Resume
                    if (isMusicRunning && isPaused) {
                        std::cout << "Resuming music..." << std::endl;
                        isPaused = false;
                    } else {
                        std::cout << "No music to resume or already playing." << std::endl;
                    }
                    break;
                case -4: // Stop
                    if (isMusicRunning) {
                        std::cout << "Stopping music..." << std::endl;
                        stopMusic = true;

                        if (musicThread.joinable()) {
                            musicThread.join(); // Đợi thread phát nhạc dừng
                        }

                        isMusicRunning = false;
                        stopMusic = false; // Đặt lại trạng thái
                    } else {
                        std::cout << "No music is playing." << std::endl;
                    }
                    break;
                case EXIT_MAIN_MENU:
                    if (isMusicRunning) {
                        std::cout << "Stopping music before exiting..." << std::endl;
                        stopMusic = true;

                        if (musicThread.joinable()) {
                            musicThread.join();
                        }

                        isMusicRunning = false;
                        stopMusic = false;
                    }
                    return;
                default:
                    throw InvalidChoiceException();
            }
        } catch (const MainMenuException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const InvalidChoiceException& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
}