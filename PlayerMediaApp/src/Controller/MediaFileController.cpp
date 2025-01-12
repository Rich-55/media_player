#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(std::shared_ptr<MediaFile>  m, std::shared_ptr<ViewBase> v) : mediaFile(m), mediaFileHandlerView(v){}

void MediaFileController::getDetailMediaFile(){
    mediaFileHandlerView->displayDetailMediaFile(mediaFile);
}

void MediaFileController::editMediaFile(){
    mediaFileHandlerView->displayMenuEditMediaFile(mediaFile);
    std::cout << "Enter the key you want to edit (or enter 0 to back to Menu): ";
    std::string key;
    std::cin >> key;

    if(key == "0"){
        return;
    }
    std::cout << "Current value: " << mediaFile->getMetadata(key) << std::endl;

    std::cout << "Enter new value: ";
    std::string newValue;
    std::cin.ignore(); 
    std::getline(std::cin, newValue);

    if (mediaFile->getAllMetadata().find(key) != mediaFile->getAllMetadata().end()) {
        mediaFile->editKey(key, newValue);
        std::cout << "Metadata updated successfully.\n";
    } else {
        std::cerr << "Error: Invalid key!\n";
    }
}

void MediaFileController::addNewKey(){
    mediaFileHandlerView->displayMenuAddNewKey(mediaFile);
    std::cout << "Enter the key you want to add: ";
    std::string key;
    std::cin >> key;

    std::cout << "Enter the value: ";
    std::string value;
    std::cin.ignore(); 
    std::getline(std::cin, value);

    mediaFile->addNewKey(key, value);
}

void MediaFileController::deleteKey(){
    mediaFileHandlerView->displayMenuDeleteKey(mediaFile);
    std::string key;
    std::cin >> key;

    if (mediaFile->getAllMetadata().find(key) != mediaFile->getAllMetadata().end()) {
        mediaFile->deleteKey(key);
        std::cout << "Key deleted successfully.\n";
    } else {
        std::cerr << "Error: Invalid key!\n";
    }
}