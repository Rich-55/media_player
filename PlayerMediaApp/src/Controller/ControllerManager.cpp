#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager model, ViewManager view,
                                     std::shared_ptr<UARTManager> uart)
    : model(model), view(view), uartManager(uart), scannerController(nullptr),
      mediaFileHandlerController(nullptr), mediaPlaylistController(nullptr),
      playerController(nullptr), mediaFileManagerController(nullptr),
      mediaPlaylistManagerController(nullptr) {}

std::shared_ptr<BaseView>
ControllerManager::getView(const std::string &viewName) 
{
  auto viewPtr = view.getView(viewName);
  if (!viewPtr) {
    throw std::runtime_error("Error: " + viewName +
                             " not found or failed to initialize.");
  }
  return viewPtr;
}
/// @brief Run UART function
bool ControllerManager::checkUart() 
{
  auto uartView = getView("UartView");
  std::vector<std::string> portList = uartManager->getPortList();
  std::vector<std::string> baudRateOptions = uartManager->getBaudRateOptions();
  std::string port;
  unsigned int baudRate;
  std::pair<std::string, unsigned int> portAndBaudRate;
  while (true) {
    portAndBaudRate = uartView->getPortAndBaudRate(portList, baudRateOptions);
    port = portAndBaudRate.first;
    baudRate = portAndBaudRate.second;
    if (port == "exit") {
      return false;
    }
        std::cout << "Port: " << port << ", Baud Rate: " << baudRate << std::endl;
    if (!uartManager->checkPortConnection(port, baudRate)) {
      std::cout << "Failed to connect to port " << port << ".\n";
      continue;
    } else {
      std::cout << "Connected to port " << port << ".\n";
      break;
    }
  }
  return true;
}

/// @brief ScanData function: Scan data from the folder in directory or USB
void ControllerManager::ScanData() 
{
  try {
    auto scanView = getView("ScanView");

    if (!scannerController) {
      scannerController = std::make_shared<MediaScannerController>(
          model.getMediaFileManager(), model.getPlaylistManager(),
          model.getFolderManager(), scanView);
    }
    scannerController->handleScan(false);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/// @brief Media File Handler function: Handle metadata file (add, delete, edit)
std::string ControllerManager::mediaFileHandler() 
{
  try {

    std::string fileName;
    auto mediaFileHandlerView = getView("MediaFileHandlerView");
    auto mediaFileManagerView = getView("MediaFileManagerView");

    if (!mediaFileManagerController) {
      mediaFileManagerController = std::make_unique<MediaFileManagerController>(
          model.getMediaFileManager(), mediaFileManagerView,
          mediaFileHandlerView, scannerController);
    }

    fileName = mediaFileManagerController->showAllMediaFile();

    if (fileName == "exit") {
      return "exit";
    }

    if (PlayerController::currentPlayingFile ==
        model.getMediaFileManager().getMediaFile(fileName)->getPath()) {
      return "File is running, can't edit";
    }

    mediaFileHandlerController = std::make_shared<MediaFileController>(
        model.getMediaFile(fileName), mediaFileHandlerView);

    mediaFileManagerController->addMediaFileController(
        fileName, mediaFileHandlerController);

    mediaFileHandlerController->handlerMediaFile();
  } catch (const std::exception &e) {
    return e.what();
  }
  return "";
}

/// @brief MediaFileManager function: Handle media file (add, delete, edit,
/// view)
void ControllerManager::mediaFileManager() 
{
  try {
    auto scanView = getView("ScanView");

    if (!scannerController) {
      scannerController = std::make_shared<MediaScannerController>(
          model.getMediaFileManager(), model.getPlaylistManager(),
          model.getFolderManager(), scanView);
    }

    auto mediaFileManagerView = getView("MediaFileManagerView");
    auto mediaFileHandlerView = getView("MediaFileHandlerView");

    if (!mediaFileManagerController) {
      mediaFileManagerController = std::make_unique<MediaFileManagerController>(
          model.getMediaFileManager(), mediaFileManagerView,
          mediaFileHandlerView, scannerController);
    }
    mediaFileManagerController->handlerMediaFileManager();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/// @brief PlaylistHandler function: Handle playlist (add media, delete media,
/// view)
void ControllerManager::playlistHandler() 
{
  try {
    std::string playlistName;

    auto playlistManagerView = getView("PlaylistManagerView");
    auto playlistHandlerView = getView("PlaylistHandlerView");
    auto mediaFileManagerView = getView("MediaFileManagerView");

    if (!mediaPlaylistManagerController) {
      mediaPlaylistManagerController =
          std::make_unique<MediaPlaylistManagerController>(
              model.getPlaylistManager(), model.getMediaFileManager(),
              model.getFolderManager(), mediaFileManagerView,
              playlistManagerView, playlistHandlerView);
    }

    if (!mediaFileManagerController) {
      auto scanView = getView("ScanView");

      if (!scannerController) {
        scannerController = std::make_shared<MediaScannerController>(
            model.getMediaFileManager(), model.getPlaylistManager(),
            model.getFolderManager(), scanView);
      }

      auto mediaFileManagerView = getView("MediaFileManagerView");
      auto mediaFileHandlerView = getView("MediaFileHandlerView");

      mediaFileManagerController = std::make_unique<MediaFileManagerController>(
          model.getMediaFileManager(), mediaFileManagerView,
          mediaFileHandlerView, scannerController);
    }

    if (!model.getPlaylistManager().checkPlaylist()) {
      std::string name;
      name = playlistManagerView->showMenuCreatePlaylist();
      if (name == "exit") {
        return;
      }
      mediaPlaylistManagerController->createPlaylist(name);
    }

    playlistName = mediaPlaylistManagerController->displayAllPlaylist();

    if (playlistName == "exit") {
      return;
    }

    mediaPlaylistController = std::make_shared<MediaPlaylistController>(
        model.getMediaFileManager(), model.getFolderManager(),
        model.getPlaylist(playlistName), mediaFileManagerView,
        playlistHandlerView);

    mediaPlaylistManagerController->addMediaPlaylistController(
        playlistName, mediaPlaylistController);

    mediaPlaylistController->handlerPlaylist();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/// @brief PlaylistManager function: Handle playlist (create, delete, view)
void ControllerManager::playlistManager() 
{
  try {
    auto playlistManagerView = getView("PlaylistManagerView");
    auto playlistHandlerView = getView("PlaylistHandlerView");
    auto mediaFileManagerView = getView("MediaFileManagerView");

    if (!mediaPlaylistManagerController) {
      mediaPlaylistManagerController =
          std::make_unique<MediaPlaylistManagerController>(
              model.getPlaylistManager(), model.getMediaFileManager(),
              model.getFolderManager(), mediaFileManagerView,
              playlistManagerView, playlistHandlerView);
    }

    if (!model.getPlaylistManager().checkPlaylist()) {
      std::string name;
      name = playlistManagerView->showMenuCreatePlaylist();
      if (name == "exit") {
        return;
      }
      mediaPlaylistManagerController->createPlaylist(name);
    }

    mediaPlaylistManagerController->handlerPlaylistManager();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/// @brief PlayMusicHandler function: Play single music
std::string ControllerManager::playMusicHandler() 
{
  try {
    auto mediaFileManagerView = getView("MediaFileManagerView");
    std::string fileName;

    fileName = mediaFileManagerView->displayAllMediaFileOfAudio(
        model.getMediaFileManager());

    if (fileName == "exit") {
      return "exit";
    }

    std::string filePath =
        model.getMediaFileManager().getMediaFile(fileName)->getPath();

    return filePath;
  } catch (const std::exception &e) {
    return e.what();
  }
}

/// @brief PlayPlaylist function: Play playlist
std::vector<std::string> ControllerManager::playPlaylist() 
{
  std::string playlistName;
  auto playlistManagerView = getView("PlaylistManagerView");
  auto playlistHandlerView = getView("PlaylistHandlerView");
  auto mediaFileManagerView = getView("MediaFileManagerView");

  if (!mediaPlaylistManagerController) {
    mediaPlaylistManagerController =
        std::make_unique<MediaPlaylistManagerController>(
            model.getPlaylistManager(), model.getMediaFileManager(),
            model.getFolderManager(), mediaFileManagerView, playlistManagerView,
            playlistHandlerView);
  }

  if (!model.getPlaylistManager().checkPlaylist()) {
    std::string name;
    name = playlistManagerView->showMenuCreatePlaylist();
    if (name == "exit") {
      return {"exit"};
    }
    mediaPlaylistManagerController->createPlaylist(name);
  }

  playlistName = mediaPlaylistManagerController->displayAllPlaylist();

  if (playlistName == "exit") {
    return {"exit"};
  }

  mediaPlaylistController = std::make_shared<MediaPlaylistController>(
      model.getMediaFileManager(), model.getFolderManager(),
      model.getPlaylist(playlistName), mediaFileManagerView,
      playlistHandlerView);

  return mediaPlaylistController->getListPathMediaFiles();
}
/// @brief PlayVideoHandler function: Play single video
std::string ControllerManager::playVideoHandler() 
{
  try {
    auto mediaFileManagerView = getView("MediaFileManagerView");
    std::string fileName;

    fileName = mediaFileManagerView->displayAllMediaFileOfVideo(
        model.getMediaFileManager());

    if (fileName == "exit") {
      return "exit";
    }

    std::string filePath =
        model.getMediaFileManager().getMediaFile(fileName)->getPath();

    return filePath;
  } catch (const std::exception &e) {
    return e.what();
  }
}

/// @brief runApp function: Run the application
void ControllerManager::runApp() 
{
  if (model.getFolderManager().getListFolderDirectory().empty() &&
      model.getFolderManager().getListFolderUSB().empty()) {
    return;
  }

  //uartManager->runMediaUart(playerController);

  auto mainMenuView = getView("MainMenuView");
  std::string typePlay = "noplay";
  int choice;
  std::string error;

  while (true) {
    try {
      if (playerController) {
        playerController->setNotificationsEnabled(true);
      }
      choice = mainMenuView->showMenuWithPlayer(
          model.getMediaFileManager(), playerController, typePlay, error);

      switch (choice) {
      case METADATA_FILE_HANDLER: 
        {
          if (playerController) {
            playerController->setNotificationsEnabled(false);
          }
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
        {
            if (playerController) {
            playerController->setNotificationsEnabled(false);
            }
            mediaFileManager();
            break;
        }
      case PLAYLIST_HANDLER:
        {
            if (playerController) {
            playerController->setNotificationsEnabled(false);
            }
            playlistHandler();
            break;
        }
      case PLAYLIST_MANAGER:
        {
            if (playerController) {
            playerController->setNotificationsEnabled(false);
            }
            playlistManager();
            break;
        }
      case PLAY_MUSIC: 
        {
            std::string music;
            if (playerController) {
            playerController->setNotificationsEnabled(false);
            }
            music = playMusicHandler();
            if (music == "exit") {
            break;
            }
            if (!music.empty()) {
            typePlay = "single";
            std::vector<std::string> singleMedia = {music};
            if (playerController) {
                playerController->stop();
                playerController = nullptr;
            }
            playerController = std::make_shared<PlayerController>(singleMedia);
            playerController->play();
            }
            break;
        }
      case PLAY_PLAYLIST: 
        {
            std::vector<std::string> listMedia;
            if (playerController) {
            playerController->setNotificationsEnabled(false);
            }
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
      case PLAY_VIDEO: 
        {
            std::string video;
            if (playerController) {
            playerController->setNotificationsEnabled(false);
            }
            video = playVideoHandler();
            if (video == "exit") {
            break;
            }
            if (!video.empty()) {
            typePlay = "single";
            std::vector<std::string> singleMedia = {video};
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
        {
            if (playerController) {
            playerController->stop();
            playerController = nullptr;
            }
            return;
        }
      default:
        throw InvalidChoiceException();
      }
    } catch (const std::exception &e) {
      error = e.what();
    }
  }
}
