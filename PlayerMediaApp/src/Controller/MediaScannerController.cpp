#include "../../include/Controller/MediaScannerController.h"

MediaScannerController::MediaScannerController(MetadataManager& metadataManager, FolderManager& folderManager,std::shared_ptr<ViewBase> scanView): metadataManager(metadataManager), folderManager(folderManager), scanView(scanView){}


std::unordered_set<std::string> MediaScannerController::getListPaths(){return this->listPaths;}

bool has_extension(const std::string &filename, const std::string &extension) 
{
    if (filename.size() >= extension.size()) {
        return (filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0);
    }
    return false;
}

std::vector<std::string> list_folders(const std::string &path) 
{
    std::vector<std::string> folders;
    DIR *dir = opendir(path.c_str());
    if (!dir) {
        throw DirectoryOpenException(path);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
            std::string full_path = path + "/" + entry->d_name;
            struct stat sb;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                folders.push_back(full_path);
            }
        }
    }
    closedir(dir);
    return folders;
}

std::vector<std::string> list_media_files(const std::string &path) 
{
    std::vector<std::string> media_files;
    DIR *dir = opendir(path.c_str());
    if (!dir) {
        throw DirectoryOpenException(path);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
            std::string full_path = path + "/" + entry->d_name;
            struct stat sb;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
                std::string filename = entry->d_name;
                if (has_extension(filename, ".mp3") || has_extension(filename, ".mp4")) {
                    media_files.push_back(full_path);  
                }
            }
        }
    }
    closedir(dir);
    return media_files;
}

std::vector<std::string> scan_all_folders(const std::string &path) 
{
    std::vector<std::string> all_media_files;
    std::vector<std::string> folders = list_folders(path);
    for (const auto &folder : folders) {
        std::vector<std::string> media_files = list_media_files(folder);
        all_media_files.insert(all_media_files.end(), media_files.begin(), media_files.end());  
    }
    return all_media_files;
}

void MediaScannerController::scanHomeDirectory() 
{
    const char *home = std::getenv("HOME");
    if (!home) {
        throw HomeDirectoryException();
    }

    std::string home_path = std::string(home);
    std::vector<std::string> folders = list_folders(home_path);

    if (folders.empty()) {
        throw NoFoldersFoundException(home_path);
    }

    std::cout << "Folders in Home:\n";
    for (size_t i = 0; i < folders.size(); ++i) {
        std::cout << i + 1 << ". " << folders[i] << '\n';
    }

    int choice = 0;
    std::cout << "Enter the number of the folder you want to scan: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(folders.size())) {
        throw InvalidChoiceException();
    }

    std::string selected_folder = folders[choice - 1];
    std::vector<std::string> media_files = list_media_files(selected_folder);
    folderManager.addDataFolderDirectory(selected_folder);

    for (const auto &file : media_files) {
        if (listPaths.find(file) == listPaths.end()) {
            listPaths.insert(file);
            std::cout << "Reading file: " << file << '\n';
        }else{
            std::cout << "File already read.\n";
        }
    }

    if (listPaths.empty()) {
        throw NoMediaFilesFoundException(selected_folder);
    }
}

void MediaScannerController::scanUSBDevices() 
{
    std::string usb_base_path = "/media/" + std::string(std::getenv("USER"));
    std::cout << "Scanning USB devices at: " << usb_base_path << std::endl;

    std::vector<std::string> usb_devices = list_folders(usb_base_path);

    if (usb_devices.empty()) {
        throw NoUSBDevicesFoundException();
    }

    std::cout << "USB Devices:\n";
    for (size_t i = 0; i < usb_devices.size(); ++i) {
        size_t lastSlashPos = usb_devices[i].find_last_of("/");
        std::string usb_name = (lastSlashPos != std::string::npos) 
                               ? usb_devices[i].substr(lastSlashPos + 1)
                               : usb_devices[i];

        std::cout << i + 1 << ". " << usb_name << '\n';
    }

    int choice = 0;
    std::cout << "Enter the number of the USB you want to scan: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(usb_devices.size())) {
        throw InvalidChoiceException();
    }

    std::string selected_usb = usb_devices[choice - 1];
    std::vector<std::string> folders = list_folders(selected_usb);
    folderManager.addDataFolderUSB(selected_usb);

    for (const auto &folder : folders) {
        std::vector<std::string> files_in_folder = list_media_files(folder);
        for (const auto &file : files_in_folder) {
            if (listPaths.find(file) == listPaths.end()) {
                listPaths.insert(file);
                std::cout << "Reading file: " << file << '\n';
            }else{
            std::cout << "File already read.\n";
            }
        }
    }

    if (listPaths.empty()) {
        throw NoMediaFilesFoundException(selected_usb);
    }
}

bool MediaScannerController::checkFolderDirectory(){ return this->folderManager.getListFolderDirectory().empty();}

bool MediaScannerController::checkFolderUSB(){ return this->folderManager.getListFolderUSB().empty();}

std::unordered_set<std::string> MediaScannerController::getlistFolderDirectory() 
{
    std::unordered_set<std::string> folderPaths = this->folderManager.getListFolderDirectory();
    std::unordered_set<std::string> mediaFiles;

    for (const auto &folder : folderPaths) {
        std::vector<std::string> files = list_media_files(folder);
        for (const auto &file : files) {
            if (listPaths.find(file) == listPaths.end()) {
                listPaths.insert(file);
                mediaFiles.insert(file);  
            }
        }
    }

    return mediaFiles;
}

std::unordered_set<std::string> MediaScannerController::getlistFolderUSB() 
{
    std::unordered_set<std::string> usbPaths = this->folderManager.getListFolderUSB();
    std::unordered_set<std::string> mediaFiles;

    for (const auto &usbFolder : usbPaths) {
        std::vector<std::string> files = list_media_files(usbFolder);
        for (const auto &file : files) {
            if (listPaths.find(file) == listPaths.end()) {
                listPaths.insert(file);
                mediaFiles.insert(file);  
            }
        }
    }

    return mediaFiles;
}

std::unordered_set<std::string> MediaScannerController::scanFolder(const std::string &path)
{
    std::unordered_set<std::string> mediaFiles;
    std::vector<std::string> files = list_media_files(path);
    for (const auto &file : files) {
        mediaFiles.insert(file);
    }
    return mediaFiles;
}