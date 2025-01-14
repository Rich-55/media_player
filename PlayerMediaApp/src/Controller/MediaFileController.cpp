#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(std::shared_ptr<MediaFile>  m, std::shared_ptr<BaseView> v) : mediaFile(m), mediaFileHandlerView(v){}

void MediaFileController::getDetailMediaFile(){
    mediaFileHandlerView->displayDetailMediaFile(mediaFile);
}

void MediaFileController::addMetadata(){
    mediaFileHandlerView->displayMenuAddMetadata(mediaFile);
    std::cout << "Enter the key you want to add (or enter 0 to back to Menu): ";
    std::string key;
    std::cin >> key;

    if(key == "0"){
        return;
    }

    std::cout << "Enter the value: ";
    std::string value;
    std::cin.ignore(); 
    std::getline(std::cin, value);

    mediaFile->addNewKey(key, value);
}

void MediaFileController::editMetadata(){
    mediaFileHandlerView->displayMenuEditMetadata(mediaFile);
    std::cout << "Enter the key you want to edit (or enter 0 to back to Menu): ";
    std::string key;
    std::cin >> key;

    if(key == "0"){
        return;
    }

    if(mediaFile->getMetadata(key) == ""){
        std::cerr << "Error: Key not found!\n";
        return;
    }

    std::cout << "Current value: " << mediaFile->getMetadata(key) << std::endl;

    std::cout << "Enter new value: ";
    std::string newValue;
    std::cin.ignore(); 
    std::getline(std::cin, newValue);

    mediaFile->editKey(key, newValue);
}

void MediaFileController::deleteMetadata(){
    mediaFileHandlerView->displayMenuDeleteMetadata(mediaFile);
    std::string key;
    std::cin >> key;

    mediaFile->deleteKey(key);
  
}

void MediaFileController::handlerMediaFile()
{
    
    int choice;
    while(true){
        getDetailMediaFile();
        mediaFileHandlerView->showMenu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            addMetadata();
            break;
        case 2:
            editMetadata();
            break;
        case 3:
            deleteMetadata();
            break;    
        case 0:
            return;
        default:
            break;
        }
    }
}