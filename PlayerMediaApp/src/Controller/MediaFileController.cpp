#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(std::shared_ptr<MediaFile>  m, std::shared_ptr<BaseView> v) : mediaFile(m), mediaFileHandlerView(v){}

void MediaFileController::getDetailMediaFile(){
    mediaFileHandlerView->displayDetailMediaFile(mediaFile);
}

void MediaFileController::addMetadata() {
    std::string key;
    while(true){
        try {
            mediaFileHandlerView->displayMenuAddMetadata(mediaFile);
            std::cout << "Enter the key you want to add (or enter 0 to back to Menu): ";
            std::cin >> key;

            if (key == "0") {
                break;
            }

            if (!isKeyAllowed(key)) {
                throw KeyNotAllowedException();
            }

            std::cout << "Enter the value: ";
            std::string value;
            std::cin.ignore(); 
            std::getline(std::cin, value);

            if (value.empty()) {
                throw InvalidValueFormatException("Value cannot be empty.", key);
            }

            if (key == "year") {
                if (!isValidFormat(value, R"(^\d{4}$)")) {
                    throw InvalidValueFormatException("Year must be a 4-digit number.", key);
                }
            } else if (key == "track") {
                if (!isValidFormat(value, R"(^\d+(/\d+)?$)")) {
                    throw InvalidValueFormatException("Track must be a positive integer or in the format x/y.", key);
                }
            }

            if(mediaFile->addNewKey(key, value)){
                std::cout << "Added new metadata [" << key << "] with value: " << value << "\n";
                break;
            }else{
                throw std::runtime_error("Key already exists.");
            }
        }
        catch (const MetadataEditException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
}

void MediaFileController::editMetadata() {
    std::string key;
    while(true){
        try {
            mediaFileHandlerView->displayMenuEditMetadata(mediaFile);
            std::cout << "Enter the key you want to edit (or enter 0 to back to Menu): ";
            std::cin >> key;

            if (key == "0") {
                break;
            }

            if (!isKeyAllowed(key)) {
                throw KeyNotAllowedException();
            }

            if (mediaFile->getMetadata(key) == "") {
                throw KeyNotFoundException(key);
            }

            std::cout << "Current value: " << mediaFile->getMetadata(key) << std::endl;

            std::cout << "Enter new value: ";
            std::string newValue;
            std::cin.ignore(); 
            std::getline(std::cin, newValue);

            if (newValue.empty()) {
                throw InvalidValueFormatException("Value cannot be empty.", key);
            }

            if (key == "year") {
                if (!isValidFormat(newValue, R"(^\d{4}$)")) {
                    throw InvalidValueFormatException("Year must be a 4-digit number.", key);
                }
            } else if (key == "track") {
                if (!isValidFormat(newValue, R"(^\d+(/\d+)?$)")) {
                    throw InvalidValueFormatException("Track must be a positive integer or in the format x/y.", key);
                }
            }

            if(mediaFile->editKey(key, newValue)){
                std::cout << "Updated metadata [" << key << "] to: " << newValue << "\n";
                break;
            }else{
                throw EditFailException();
            }
        } catch (const MetadataEditException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
}

void MediaFileController::deleteMetadata() {
    std::string key;
    while(true)
    {
        try {
            mediaFileHandlerView->displayMenuDeleteMetadata(mediaFile);
            std::cout << "Enter the key you want to delete (or enter 0 to back to Menu): ";

            std::cin >> key;

            if (key == "0") {
                break;
            }

            if (!isKeyAllowed(key)) {
                throw KeyNotAllowedException();
            }

            if (mediaFile->getMetadata(key) == "") {
                throw KeyNotFoundException(key);
            }

            if(mediaFile->deleteKey(key)){
                std::cout << "Deleted metadata [" << key << "]\n";
                break;
            }else{
                throw EditFailException();
            }
        } catch (const MetadataEditException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
}

void MediaFileController::handlerMediaFile()
{ 
    int choice;
    while(true){
        try{
            getDetailMediaFile();
            mediaFileHandlerView->showMenu();
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                std::cerr << "Invalid input. Please enter a number.\n";
                continue; 
            }
            switch (choice)
            {
            case ADD_METADATA:
                addMetadata();
                break;
            case EDIT_METADATA:
                editMetadata();
                break;
            case DELETE_METADATA:
                deleteMetadata();
                break;    
            case EXIT_MENU_METADATA_EDIT:
                return;
            default:
                throw InvalidChoiceException();
            }
        }catch(const MetadataEditException& e){
            std::cerr << "Error: " << e.what() << std::endl;
        }catch (const InvalidChoiceException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}