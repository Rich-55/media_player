#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(std::shared_ptr<MediaFile>  m, std::shared_ptr<BaseView> v) : mediaFile(m), mediaFileHandlerView(v)
{}

void MediaFileController::getDetailMediaFile(std::string message) {mediaFileHandlerView->displayDetailMediaFile(mediaFile, message);}

std::string MediaFileController::addMetadata() 
{
    std::pair<std::string, std::string> key_value;
    std::string exception;

    while(true){
        try {
            key_value = mediaFileHandlerView->displayMenuAddMetadata(mediaFile, exception);

            if(key_value.first == "0"){
                break;
            }

            if (key_value.second.empty()) {
                throw InvalidValueFormatException("Value cannot be empty.", key_value.first);
            }

            if (key_value.first == "year") {
                if (!isValidFormat(key_value.second, R"(^\d{4}$)")) {
                    throw InvalidValueFormatException("Year must be a 4-digit number.", key_value.first);
                }
            } else if (key_value.first == "track") {
                if (!isValidFormat(key_value.second, R"(^\d+(/\d+)?$)")) {
                    throw InvalidValueFormatException("Track must be a positive integer or in the format x/y.", key_value.first);
                }
            }

            if(mediaFile->addNewKey(key_value.first, key_value.second)){
                return "Added new metadata [" + key_value.first + "] with value: " + key_value.second;
            }else{
                throw std::runtime_error("Key already exists.");
            }
        }
        catch (const MetadataEditException& e) {
            exception = e.what();
        }catch (const std::exception& e) {
            exception = e.what();
        }
    }
    return "";
}

std::string MediaFileController::editMetadata() 
{
    std::pair<std::string, std::string> key_value;
    std::string exception;

    while(true){
        try {
            key_value = mediaFileHandlerView->displayMenuEditMetadata(mediaFile, exception);
            
            if(key_value.first == "0"){
                break;
            }

            if (key_value.second.empty()) {
                throw InvalidValueFormatException("Value cannot be empty.", key_value.first);
            }

            if (key_value.first == "year") {
                if (!isValidFormat(key_value.second, R"(^\d{4}$)")) {
                    throw InvalidValueFormatException("Year must be a 4-digit number.", key_value.first);
                }
            } else if (key_value.first == "track") {
                if (!isValidFormat(key_value.second, R"(^\d+(/\d+)?$)")) {
                    throw InvalidValueFormatException("Track must be a positive integer or in the format x/y.", key_value.first);
                }
            }

            if(mediaFile->editKey(key_value.first, key_value.second)){
                return "Updated metadata [" + key_value.first + "] to: " + key_value.second;
                
            }else{
                throw EditFailException();
            }
        } catch (const MetadataEditException& e) {
            exception = e.what();
        }
    }
    return "";
}

std::string MediaFileController::deleteMetadata() 
{
    std::string key;

    while(true)
    {
        try {
            key = mediaFileHandlerView->displayMenuDeleteMetadata(mediaFile);

            if (key == "0") {
                break;
            }

            if(mediaFileHandlerView->showConfirmMenu("Are you sure you want to delete this metadata [" + key + "] ?")){
                if(mediaFile->deleteKey(key)){
                    return "Deleted metadata [" + key + "]";
                }else{
                    throw EditFailException();
                }
            }else{
                continue;
            }
        } catch (const MetadataEditException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    return "";
}   

void MediaFileController::handlerMediaFile()
{   
    std::string message;
    
    while(true){
        try{
            getDetailMediaFile(message);
            int choice = mediaFileHandlerView->showMenu();
            switch (choice)
            {
            case ADD_METADATA:
                message = addMetadata();
                break;
            case EDIT_METADATA:
                message = editMetadata();
                break;
            case DELETE_METADATA:
                message = deleteMetadata();
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