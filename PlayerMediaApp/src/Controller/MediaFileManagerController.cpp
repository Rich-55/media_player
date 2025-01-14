#include "../../include/Controller/MediaFileManagerController.h"

MediaFileManagerController::MediaFileManagerController(MediaFileManager& m, std::shared_ptr<BaseView> v, std::shared_ptr<MediaScannerController> c) : mediaManager(m), mediaFileManagerView(v), scannerController(c){}

void MediaFileManagerController::addDataFolder(const std::unordered_set<std::string> &listPathName) {
    for (const auto &path : listPathName) {
        std::cout << "Reading file: " << path << '\n';
        size_t lastSlashPos = path.find_last_of("/");
        std::string fileName = (lastSlashPos != std::string::npos) ? path.substr(lastSlashPos + 1) : path;

        size_t lastDotPos = path.find_last_of(".");
        std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

        if (extension == "mp4") {
            mediaManager.addMediaFile(path, "Video");
        } else if (extension == "mp3") {
            mediaManager.addMediaFile(path, "Audio");
        } else {
            std::cerr << "Unsupported file type: " << fileName << '\n';
            continue;
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

        if (extension == "mp4") {
            mediaManager.loadMediaFile(path, "Video");
        } else if (extension == "mp3") {
            mediaManager.loadMediaFile(path, "Audio");
        } else {
            std::cerr << "Unsupported file type: " << fileName << '\n';
            continue;
        }
    }
}

void MediaFileManagerController::addDataFile(std::string pathName) {

    size_t lastSlashPos = pathName.find_last_of("/");
    size_t lastDotPos = pathName.find_last_of(".");

    std::string fileName = (lastSlashPos != std::string::npos) ? pathName.substr(lastSlashPos + 1) : pathName;
    std::string extension = (lastDotPos != std::string::npos) ? pathName.substr(lastDotPos + 1) : "";
    if(extension == "mp4"){
        mediaManager.addMediaFile(pathName, "Video");
    }else if(extension == "mp3"){
        mediaManager.addMediaFile(pathName, "Audio");
    }else {
        std::cerr << "Unsupported file type: " << fileName << '\n';
    }
}

std::unordered_set<std::string> MediaFileManagerController::getListFileAdded(){ return mediaManager.getListFileAdded();}

void MediaFileManagerController::clearListFileAdded(){ mediaManager.clearListFileAdded();}

void MediaFileManagerController::deleteData(std::string fileName) { mediaManager.deleteMediaFile(fileName);}

void MediaFileManagerController::showAllMediaFile(){ mediaFileManagerView->displayAllMediaFile(mediaManager);}

void MediaFileManagerController::showAllMediaFileOfVideo(){ mediaFileManagerView->displayAllMediaFileOfVideo(mediaManager);}

void MediaFileManagerController::showAllMediaFileOfAudio(){ mediaFileManagerView->displayAllMediaFileOfAudio(mediaManager);}

void MediaFileManagerController::handleMediaFileManager()
{
    int choice; 
    while(true){
        mediaFileManagerView->showMenu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            {
            std::cout << "Enter your path of file to add: ";
            std::string pathFile;
            std::cin >> pathFile;
            addDataFile(pathFile);
            break;
            }
        case 2:
            {
            scannerController->handleScan(true);
            break;
            }
        case 3:
            {
            std::cout << "Enter the name of the file you want to delete: ";
            std::string fileName;
            std::cin >> fileName;
            deleteData(fileName);
            break;
            }
        case 4:
            showAllMediaFile();
            break;
        case 5:
            showAllMediaFileOfAudio();
            break;
        case 6:
            showAllMediaFileOfVideo();
            break;    
        case 0:
            return;
        default:
            break;
        }
    }
}