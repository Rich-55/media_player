#include "../../include/Controller/MediaFileManagerController.h"

MediaFileManagerController::MediaFileManagerController(MediaFileManager& mediaManager, std::shared_ptr<BaseView> mediaManagerView, std::shared_ptr<BaseView> mediaFileView,std::shared_ptr<MediaScannerController> c) 
    : mediaManager(mediaManager), mediaFileManagerView(mediaManagerView), mediaFileHandlerView(mediaFileView), scannerController(c)
{}

void MediaFileManagerController::addMediaFileController(std::string fileName, std::shared_ptr<MediaFileController> mediaFileController)
{
    if(listMediaFileController.find(fileName) == listMediaFileController.end()){
        listMediaFileController[fileName] = mediaFileController;
    }
}

std::shared_ptr<MediaFileController> MediaFileManagerController::getMediaFileController(const std::string& fileName) 
{
    auto currentMediaFileController = listMediaFileController.find(fileName);
    if (currentMediaFileController == listMediaFileController.end()) {
        std::cerr << "Error: MediaFileController not found.\n";
        return nullptr;
    }
   return currentMediaFileController->second;
}

void MediaFileManagerController::addDataFolder(const std::unordered_set<std::string> &listPathName) 
{
    for (const auto &path : listPathName) {

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

bool MediaFileManagerController::deleteData(std::string fileName) 
{
    if(mediaFileManagerView->showConfirmMenu("Do you want to delete this file: " + fileName + " ?")){
        return mediaManager.deleteMediaFile(fileName);
    }
    return false;
}

std::string MediaFileManagerController::showAllMediaFile(){ return mediaFileManagerView->displayAllMediaFile(mediaManager);}

std::string MediaFileManagerController::showAllMediaFileOfAudio(){ return mediaFileManagerView->displayAllMediaFileOfAudio(mediaManager);}

std::string MediaFileManagerController::showAllMediaFileOfVideo(){ return mediaFileManagerView->displayAllMediaFileOfVideo(mediaManager);}


void MediaFileManagerController::handleMediaFileManager()
{
    int choice;
    std::string message;
    std::string error;
    
    while (true) {
        system("clear");
        if(!message.empty()){
            mediaFileManagerView->showNotificationMessage(message, "success");
            message = "";
        }
        if(!error.empty()){
            mediaFileManagerView->showNotificationMessage(error, "error");
            error = "";
        }
        
        choice = mediaFileManagerView->showMenuWithMediaList(mediaManager);
        try {
            switch (choice) {
                case ADD_FILE_PATH: {
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
                            } 
                            message = "File " + pathFile + " has been added.";
                            break;
                        } catch (const MediaFileManagerException &e) {
                            error =  e.what();
                        }
                  break;
                }
                case ADD_FOLDER_PATH: {
                    scannerController->handleScan(true);
                    break;
                }
                case DELETE_FILE: {
                     while (true) {
                        std::string fileName;
                        fileName = showAllMediaFile();
                        if (fileName == "exit") {
                            break;
                        }
                        if(deleteData(fileName)){
                            message = "File " + fileName + " has been deleted.";
                        }else{
                            continue;
                        }
                        break; 
                    }
                    break;
                }
                case SHOW_ALL_MEDIAFILE:
                {
                    std::string filename;
                    filename =  showAllMediaFile();
                    if(filename == "exit"){
                        break;
                    }
                    if(listMediaFileController.find(filename) == listMediaFileController.end()){
                        std::shared_ptr<MediaFileController> mediaFileController = std::make_shared<MediaFileController>(mediaManager.getMediaFile(filename), mediaFileHandlerView);
                        listMediaFileController[filename] = mediaFileController;
                    }
                    listMediaFileController[filename]->handlerMediaFile();
                    break;
                }
                case SHOW_ALL_MEDIAFILE_AUDIO:
                {
                    std::string filename;
                    filename = showAllMediaFileOfAudio();
                    if(filename == "exit"){
                        break;
                    }
                    if(listMediaFileController.find(filename) == listMediaFileController.end()){
                        std::shared_ptr<MediaFileController> mediaFileController = std::make_shared<MediaFileController>(mediaManager.getMediaFile(filename), mediaFileHandlerView);
                        addMediaFileController(filename, mediaFileController);
                    }
                    listMediaFileController[filename]->handlerMediaFile();
                    break;
                }
                case SHOW_ALL_MEDIAFILE_VIDEO:
                {
                    std::string filename;
                    filename = showAllMediaFileOfVideo();
                    if(filename == "exit"){
                        break;
                    }
                    if(listMediaFileController.find(filename) == listMediaFileController.end()){
                        std::shared_ptr<MediaFileController> mediaFileController = std::make_shared<MediaFileController>(mediaManager.getMediaFile(filename), mediaFileHandlerView);
                        addMediaFileController(filename, mediaFileController);
                    }
                    listMediaFileController[filename]->handlerMediaFile();
                    break;
                }
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
