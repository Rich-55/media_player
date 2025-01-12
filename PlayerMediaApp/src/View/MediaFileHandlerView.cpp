#include "../../include/View/MediaFileHandlerView.h"

MediaFileHandlerView::MediaFileHandlerView(){}

void MediaFileHandlerView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    std::cout << "1. Add metadata" << std::endl;
    std::cout << "2. Edit metadata" << std::endl;
    std::cout << "3. Delete metadata" << std::endl;
    std::cout << "0. Back to main menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

void MediaFileHandlerView::displayDetailMediaFile(std::shared_ptr<MediaFile> mediaFile){
    std::cout << "File Name:   " << mediaFile->getName() << std::endl;
    std::cout << "File Path:   " << mediaFile->getPath() << std::endl;
    std::cout << "File Type:   " << mediaFile->getType() << std::endl;
    std::cout << "File Size:   " << mediaFile->getSize() << std::endl;
    if(mediaFile->getType() == "Audio"){
        std::cout << "Audio Metadata:" << std::endl;
        for (const auto& [key, value] : mediaFile->getAllMetadata()) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }else{
        std::cout << "Video Metadata:" << std::endl;
        for (const auto& [key, value] : mediaFile->getAllMetadata()) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }
    std::cout << std::string(50, '-') << std::endl;
}

void MediaFileHandlerView::displayMenuEditMetadata(std::shared_ptr<MediaFile> mediaFile)
{
    if(mediaFile->getType() == "Audio"){
        std::cout << "Editing media file: " << mediaFile->getName() << ": " << std::endl;
        for (const auto& [key, value] : mediaFile->getAllMetadata()) {
            std::cout <<  " " << "Edit " << key << ": " << value << std::endl;
        }
    }else{
        std::cout << "Editing media file: " << mediaFile->getName() << ": " << std::endl;
        for (const auto& [key, value] : mediaFile->getAllMetadata()) {
            std::cout <<  " " << "Edit " << key << ": " << value << std::endl;
        }
    }

}

void MediaFileHandlerView::displayMenuAddMetadata(std::shared_ptr<MediaFile> mediaFile){
    std::cout << "Add new key media file: " << mediaFile->getName() << ": " << std::endl;
    //list key can be added in taglib 
    std::cout << "List key can be added in taglib: " << std::endl;;
    std::cout << "title" << std::endl;
    std::cout << "artist" << std::endl;
    std::cout << "album" << std::endl;
    std::cout << "genre" << std::endl;
    std::cout << "comment" << std::endl;
    std::cout << "year" << std::endl;
    std::cout << "track" << std::endl;
    std::cout << "Enter the key you want to add: ";

    
}

void MediaFileHandlerView::displayMenuDeleteMetadata(std::shared_ptr<MediaFile> mediaFile){
    std::cout << "Delete key media file: " << mediaFile->getName() << std::endl;
    std::cout << "List key can be delete in taglib: " << std::endl;;
    std::cout << "title" << std::endl;
    std::cout << "artist" << std::endl;
    std::cout << "album" << std::endl;
    std::cout << "genre" << std::endl;
    std::cout << "comment" << std::endl;
    std::cout << "year" << std::endl;
    std::cout << "track" << std::endl;
    std::cout << "Enter the key you want to delete: ";
}