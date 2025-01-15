#include "../../include/Controller/MediaFileManagerController.h"

MediaFileManagerController::MediaFileManagerController(MediaFileManager& m, std::shared_ptr<BaseView> v, std::shared_ptr<MediaScannerController> c) : mediaManager(m), mediaFileManagerView(v), scannerController(c){}

void MediaFileManagerController::addDataFolder(const std::unordered_set<std::string> &listPathName) {
    for (const auto &path : listPathName) {
        std::cout << "Reading file: " << path << '\n';

        size_t lastSlashPos = path.find_last_of("/");
        std::string fileName = (lastSlashPos != std::string::npos) ? path.substr(lastSlashPos + 1) : path;

        size_t lastDotPos = path.find_last_of(".");
        std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

        try {
            if (extension == "mp4") {
                if (!mediaManager.addMediaFile(path, "Video")) {
                    throw FileAlreadyExistsException(fileName);
                }
            } else if (extension == "mp3") {
                if (!mediaManager.addMediaFile(path, "Audio")) {
                    throw FileAlreadyExistsException(fileName);
                }
            } else {
                throw UnsupportedFileTypeException(fileName);
            }
        } catch (const MediaFileManagerException &e) {
            std::cerr << e.what() << '\n';
        }
    }
}

void MediaFileManagerController::loadData(const std::unordered_set<std::string> &listPathName) {
    for (const auto &path : listPathName) {
        std::cout << "Reading file: " << path << '\n';

        size_t lastSlashPos = path.find_last_of("/");
        std::string fileName = (lastSlashPos != std::string::npos) ? path.substr(lastSlashPos + 1) : path;

        size_t lastDotPos = path.find_last_of(".");
        std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

        try {
            if (extension == "mp4") {
                if (!mediaManager.loadMediaFile(path, "Video")) {
                    throw FileNotFoundException(fileName);
                }
            } else if (extension == "mp3") {
                if (!mediaManager.loadMediaFile(path, "Audio")) {
                    throw FileNotFoundException(fileName);
                }
            } else {
                throw UnsupportedFileTypeException(fileName);
            }
        } catch (const MediaFileManagerException &e) {
            std::cerr << e.what() << '\n';
        }
    }
}

int MediaFileManagerController::addDataFile(std::string pathName)
{
    size_t lastSlashPos = pathName.find_last_of("/");
    size_t lastDotPos = pathName.find_last_of(".");

    std::string fileName = (lastSlashPos != std::string::npos) ? pathName.substr(lastSlashPos + 1) : pathName;
    std::string extension = (lastDotPos != std::string::npos) ? pathName.substr(lastDotPos + 1) : "";

    if (extension == "mp4") {
        return mediaManager.addMediaFile(pathName, "Video");
    } else if (extension == "mp3") {
        return mediaManager.addMediaFile(pathName, "Audio");
    } else {
        return -1;
    }
}

bool MediaFileManagerController::deleteData(std::string fileName) {return mediaManager.deleteMediaFile(fileName);}

std::unordered_set<std::string> MediaFileManagerController::getListFileAdded(){ return mediaManager.getListFileAdded();}

void MediaFileManagerController::clearListFileAdded(){ mediaManager.clearListFileAdded();}

void MediaFileManagerController::showAllMediaFile(){ mediaFileManagerView->displayAllMediaFile(mediaManager);}

void MediaFileManagerController::showAllMediaFileOfVideo(){ mediaFileManagerView->displayAllMediaFileOfVideo(mediaManager);}

void MediaFileManagerController::showAllMediaFileOfAudio(){ mediaFileManagerView->displayAllMediaFileOfAudio(mediaManager);}

void MediaFileManagerController::handleMediaFileManager() {
    int choice;
    while (true) {
        mediaFileManagerView->showMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cerr << "Invalid input. Please enter a number.\n";
            continue; 
        }

        try {
            switch (choice) {
                case ADD_FILE_PATH: {
                    while (true) {
                        try {
                            std::cout << "Enter your path of file to add (or press 0 to return to the menu): ";
                            std::string pathFile;
                            if (std::cin.peek() == '\n') {
                                std::cin.ignore();
                            }
                            std::getline(std::cin, pathFile);
                            if (pathFile == "0") {
                                std::cout << "Returning to the menu.\n";
                                break;
                            }

                            if (pathFile.empty() || pathFile[0] != '/') {
                                throw InvalidFilePathException(pathFile);
                            }

                            if (!isValidPath(pathFile)) {
                                throw InvalidFilePathException(pathFile);
                            }
                            if (!fileExists(pathFile)) {
                                throw FileNotFoundException(pathFile);
                            }

                            int result = addDataFile(pathFile);
                            if (result == -1) {
                                throw UnsupportedFileTypeException(pathFile);
                            } else {
                               throw FileAlreadyExistsException(pathFile);
                            }
                            break;
                        } catch (const MediaFileManagerException &e) {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    break;
                }
                case ADD_FOLDER_PATH: {
                    scannerController->handleScan(true);
                    break;
                }
                case DELETE_FILE: {
                     while (true) {
                        try {
                            std::cout << "Enter the name of the file you want to delete (or press 0 to return to the menu): ";
                            std::string fileName;
                            if (std::cin.peek() == '\n') {
                                std::cin.ignore();
                            }
                            std::getline(std::cin, fileName);

                            if (fileName == "0") {
                                std::cout << "Returning to the menu.\n";
                                break;
                            }

                            if (fileName.empty()) {
                                throw InvalidFilePathException("File name cannot be empty.");
                            }

                            if (!deleteData(fileName)) {
                                throw FileNotFoundException(fileName);
                            }
                            break; 
                        } catch (const MediaFileManagerException &e) {
                            std::cerr << e.what() << '\n'; 
                        }
                    }
                    break;
                }
                case SHOW_ALL_MEDIAFILE:
                    showAllMediaFile();
                    break;
                case SHOW_ALL_MEDIAFILE_AUDIO:
                    showAllMediaFileOfAudio();
                    break;
                case SHOW_ALL_MEDIAFILE_VIDEO:
                    showAllMediaFileOfVideo();
                    break;
                case EXIT_MENU_MEDIAFILE_MANAGER:
                    return;
                default:
                    throw InvalidChoiceException();
            }
        } catch (const MediaFileManagerException &e) {
            std::cerr << "Error: " << e.what() << '\n';
        }catch (const InvalidChoiceException &e) {
            std::cerr << "Error: " << e.what() << '\n'; 
        }
    }
}
