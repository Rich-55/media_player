#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m, CLIManager v) 
    : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistController(nullptr), mediaPlaylistManagerController(nullptr) {}

void ControllerManager::ScanData() 
{   
    try {
        if(model.getFolderManager().getListFolderDirectory().empty() && model.getFolderManager().getListFolderUSB().empty()){
            std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

            if (!scanView) {
                throw std::runtime_error("Error: ScanView not found or failed to initialize.");
            }
            if (!scannerController) {
                scannerController = std::make_unique<MediaScannerController>(model.getMetadataManager(), model.getFolderManager(), scanView);
            }

            std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
            
            if (!mediaFileManagerView) {
                throw std::runtime_error("Error: MediaFileManagerView not found or failed to initialize.");
            }
            if(!mediaFileManagerController){
                mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
            }

            while(true){
                if(!scannerController->getListPaths().empty()){
                    scanView->setListPathNameIsAdded(mediaFileManagerController->getListFileAdded());
                    mediaFileManagerController->clearListFileAdded();
                }
                view.switchView("ScanView");
                int choice; 
                std::cin >> choice;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw InvalidChoiceException();
                }

                switch (choice)
                {
                    case ScanHomeDirectory:
                        scannerController->scanHomeDirectory();
                        mediaFileManagerController->addData(scannerController->getListPaths());
                        break;
                    case 2:
                        scannerController->scanUSBDevices();
                        mediaFileManagerController->addData(scannerController->getListPaths());
                        break;
                    case 0:
                        return;
                    default:
                        throw InvalidChoiceException();
                        break;
                }
            }
        } else {
            try {
                std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

                if (!scanView) {
                    throw std::runtime_error("Error: ScanView not found or failed to initialize.");
                }
                if (!scannerController) {
                    scannerController = std::make_unique<MediaScannerController>(model.getMetadataManager(), model.getFolderManager(), scanView);
                }

                std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
                
                if (!mediaFileManagerView) {
                    throw std::runtime_error("Error: MediaFileManagerView not found or failed to initialize.");
                }
                if(!mediaFileManagerController){
                    mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
                }
                if(!scannerController->checkFolderDirectory()){
                    mediaFileManagerController->addData(scannerController->getlistFolderDirectory());
                }
                if(!scannerController->checkFolderUSB()){
                    mediaFileManagerController->addData(scannerController->getlistFolderUSB());
                }
            } catch (const ScanException& e) {
                std::cerr << "Media Scanner Error: " << e.what() << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Runtime Error: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
            }
        }
    } catch (const ScanException& e) {
        std::cerr << "Media Scanner Error: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }
}

void ControllerManager::mediaFileManager() {
    try {
        std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");

        if (!mediaFileManagerView) {
            throw std::runtime_error("Error: MediaFileManagerView not found or failed to initialize.");
        }

        if (!mediaFileManagerController) {
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
        }

        int choice;
        while (true) {
            view.switchView("MediaFileManagerView");
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid input. Please enter a number." << std::endl;
                continue;
            }

            switch (choice) {
                case 1: {
                    std::cout << "Enter your path of file to add: ";
                    std::string pathFile;
                    std::cin >> pathFile;
                    mediaFileManagerController->addDataFile(pathFile);
                    break;
                }
                case 2: {
                    std::cout << "Enter your folder path to add: ";
                    std::string pathFolder;
                    std::cin >> pathFolder;
                    mediaFileManagerController->addData(scannerController->scanFolder(pathFolder));
                    break;
                }
                case 3: {
                    std::cout << "Enter the name of the file you want to delete: ";
                    std::string fileName;
                    std::cin >> fileName;
                    mediaFileManagerController->deleteData(fileName);
                    break;
                }
                case 4:
                    mediaFileManagerController->showAllMediaFile();
                    break;
                case 5:
                    mediaFileManagerController->showAllMediaFileOfAudio();
                    break;
                case 6:
                    mediaFileManagerController->showAllMediaFileOfVideo();
                    break;
                case 0:
                    return;
                default:
                    std::cerr << "Invalid choice. Please enter a number between 0 and 6." << std::endl;
                    break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void ControllerManager::metadataFileHandler() {
    try {
        std::shared_ptr<ViewBase> mediaFileHandlerView = view.getView("MediaFileHandlerView");

        if (!mediaFileHandlerView) {
            throw std::runtime_error("Error: MediaFileHandlerView not found or failed to initialize.");
        }

        if (mediaFileManagerController) {
            mediaFileManagerController->showAllMediaFile();
        }

        std::cout << "Enter the name of the file you want to edit: ";
        std::string fileName;
        std::cin >> fileName;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Invalid file name entered.");
        }

        if (!mediaFileHandlerController) {
            std::shared_ptr<MediaFile> file = model.getMediaFile(fileName);
            if (!file) {
                throw FileNotFoundException(fileName);
            }
            mediaFileHandlerController = std::make_unique<MediaFileController>(file, mediaFileHandlerView);
        }

        int choice;
        while (true) {
            mediaFileHandlerController->getDetailMediaFile();
            view.switchView("MediaFileHandlerView");
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid input. Please enter a number." << std::endl;
                continue;
            }

            switch (choice) {
                case 1:
                    mediaFileHandlerController->addMetadata();
                    break;
                case 2:
                    mediaFileHandlerController->editMetadata();
                    break;
                case 3:
                    mediaFileHandlerController->deleteMetadata();
                    break;
                case 0:
                    return;
                default:
                    std::cerr << "Invalid choice. Please enter a number between 0 and 3." << std::endl;
                    break;
            }
        }
    } catch (const FileNotFoundException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const NoMediaFilesException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void ControllerManager::playlistManager()
{
    std::shared_ptr<ViewBase> playlistManagerView = view.getView("PlaylistManagerView");

    if (!playlistManagerView) {
        std::cerr << "Error: PlaylistManagerView not found or failed to initialize.\n";
        return;
    }

    if (!mediaPlaylistManagerController) {
        mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(model.getPlaylistManager(), playlistManagerView);
    }

    if (!model.getPlaylistManager().checkPlaylist()) {
        std::cout << "No playlists found. Do you want to create a new playlist? (Y/N): ";
        char choice;
        std::cin >> choice;

        if (choice == 'N' || choice == 'n') {
            return; 
        }

        mediaPlaylistManagerController->createPlaylist(); 
    }

    int choice;
    while (true) {
        view.switchView("PlaylistManagerView");
        std::cin >> choice;

        switch (choice) {
        case 1:
            mediaPlaylistManagerController->createPlaylist(); 
            break;
        case 2:
        {
            mediaPlaylistManagerController->deletePlaylist();
            break;
        }
        case 3:
            mediaPlaylistManagerController->displayAllPlaylist(); 
            break;
        case 0:
            return; 
        default:
            std::cerr << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void ControllerManager::playlistHandler() {
    std::shared_ptr<ViewBase> playlistManagerView = view.getView("PlaylistManagerView");

    if (!playlistManagerView) {
        std::cerr << "Error: PlaylistHandlerView not found or failed to initialize.\n";
        return;
    }

    if (!mediaPlaylistManagerController) {
        mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(model.getPlaylistManager(), playlistManagerView);
    }

    if (!model.getPlaylistManager().checkPlaylist()) {
        std::cout << "Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ";
        
        std::string choice;
        std::cin.ignore();
        getline(std::cin, choice);

        if (choice == "N" || choice == "n") {
            return; 
        }

        mediaPlaylistManagerController->createPlaylist(); 
    }

    mediaPlaylistManagerController->displayAllPlaylist();

    std::shared_ptr<ViewBase> playlistHandlerView = view.getView("PlaylistHandlerView");

    if (!playlistHandlerView) {
        std::cerr << "Error: PlaylistHandlerView not found or failed to initialize.\n";
        return;
    }

    std::cout << "Enter the name of the playlist you want to edit: ";
    std::string playlistName;
    std::cin >> playlistName;

    if (!model.getPlaylistManager().checkPlaylistName(playlistName)) {
        std::cerr << "Playlist not found!\n";
        return;
    }

    if (!mediaPlaylistController) {
        mediaPlaylistController = std::make_unique<MediaPlaylistController>(model.getPlaylist(playlistName), playlistHandlerView);
    }

    int choice;
    while (true) {
        view.switchView("PlaylistHandlerView");
        std::cin >> choice;
        switch (choice) {
        case 1:
        {
            std::cout << "Enter the name of the file you want to add: ";
            std::string fileName;
            std::cin >> fileName;
            std::shared_ptr<MediaFile> mediaFile = model.getMediaFile(fileName);
            if(mediaFile == nullptr){
                std::cerr << "File not found!\n";
                return;
            }
            mediaPlaylistController->addMediaFile(mediaFile);
        }
            
            break;
        case 2:
            mediaPlaylistController->deleteMediaFile();
            break;
        case 3:
            mediaPlaylistController->displayAllMediaFiles();
            break;
        case 0:
            return; 
        default:
            std::cerr << "Invalid choice. Please try again.\n";
            break;
        }
    }
}
