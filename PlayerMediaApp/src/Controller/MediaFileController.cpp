#include "../../include/Controller/MediaFileController.h"

MediaFileController::MediaFileController(std::shared_ptr<MediaFile>  m, std::shared_ptr<ViewBase> v) : mediaFile(m), mediaFileHandlerView(v){}

void MediaFileController::getDetailMediaFile(){
    mediaFileHandlerView->displayDetailMediaFile(mediaFile);
}

bool MediaFileController::isKeyAllowed(const std::string& key) const {
    for (const std::string& allowedKey : allowedKeys) {
        if (key == allowedKey) {
            return true;
        }
    }
    return false;
}

void MediaFileController::validateYear(const std::string& year, const std::string& key) const {
    std::regex yearRegex(R"(^\d{4}$)");
    if (!std::regex_match(year, yearRegex)) {
        throw InvalidValueFormatException("Year must be a 4-digit number.", key);
    }
}

void MediaFileController::validateStringNotEmpty(const std::string& value, const std::string& key) const {
    if (value.empty()) {
        throw InvalidValueFormatException("Value cannot be empty.", key);
    }
}

void MediaFileController::addMetadata() {
    while (true) {
        try {
            mediaFileHandlerView->displayMenuAddMetadata(mediaFile);
            std::cout << "Enter the key you want to add (or enter 0 to back to Menu): ";
            std::string key;
            std::cin >> key;

            if (key == "0") {
                return;
            }
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InvalidInputException();
            }
            if (!isKeyAllowed(key)) {
                throw InvalidInputException();
            }

            std::cout << "Enter the value: ";
            std::string value;
            std::cin.ignore();
            std::getline(std::cin, value);

            if (key == "year") {
                validateYear(value, key);
            } else if (key == "title" || key == "track" || key == "artist" || key == "album" || key == "genre" || key == "comment") {
                validateStringNotEmpty(value, key);
            }

            mediaFile->addNewKey(key, value);
            break;
        } catch (const InvalidInputException& e) {
            std::cerr << "Invalid key entered. Please choose from the allowed keys." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } catch (const MetadataEditException& e) {
            std::cerr << "Error adding metadata: " << e.what() << std::endl;
            break;
        }
    }
}

void MediaFileController::editMetadata() {
    while (true) {
        try {
            mediaFileHandlerView->displayMenuEditMetadata(mediaFile);
            std::cout << "Enter the key you want to edit (or enter 0 to go back): ";
            std::string key;
            std::cin >> key;

            if (key == "0") {
                return;
            }

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InvalidInputException();
            }

            if (mediaFile->getMetadata(key) == "") {
                throw KeyNotFoundException(key);
            }

            std::cout << "Current value: " << mediaFile->getMetadata(key) << std::endl;

            std::cout << "Enter new value: ";
            std::string newValue;
            std::cin.ignore();
            std::getline(std::cin, newValue);

            if (key == "year") {
                validateYear(newValue, key);
            } else if (key == "title" || key == "track" || key == "artist" || key == "album" || key == "genre" || key == "comment") {
                validateStringNotEmpty(newValue, key);
            }

            mediaFile->editKey(key, newValue);
            break;
        } catch (const KeyNotFoundException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const InvalidInputException& e) {
            std::cerr << "Error: Invalid input. Please enter a valid key." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } catch (const MetadataEditException& e) {
            std::cerr << "Error editing metadata: " << e.what() << std::endl;
            break;
        }
    }
}

void MediaFileController::deleteMetadata() {
    while (true) {
        try {
            mediaFileHandlerView->displayMenuDeleteMetadata(mediaFile);
            std::string key;
            std::cin >> key;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InvalidInputException();
            }

            if (!isKeyAllowed(key)) {
                throw InvalidInputException();
            }

            if (mediaFile->getMetadata(key) == "") {
                throw KeyNotFoundException(key);
            }

            mediaFile->deleteKey(key);
            break;

        } catch (const KeyNotFoundException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const InvalidInputException& e) {
            std::cerr << "Error: Invalid input. Please enter a valid key." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } catch (const MetadataEditException& e) { // Make sure this is the correct base class
            std::cerr << "Error deleting metadata: " << e.what() << std::endl;
            break;
        }
    }
}
