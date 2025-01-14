#include "../../include/Controller/MediaScannerController.h"

MediaScannerController::MediaScannerController(MediaFileManager& m, FolderManager& f,std::shared_ptr<BaseView> v) : mediaFileManager(m), folderManager(f), scanView(v){}

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
        std::cerr << "Error opening directory: " << path << '\n';
        return folders;
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
        std::cerr << "Error opening directory: " << path << '\n';
        return media_files;
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

void MediaScannerController::addDataFileWithFolder(std::string nameFolder, std::string nameLocation) 
{
    std::unordered_set<std::string> listPath;

    if (nameLocation == "Directory") {
        listPath = folderManager.getListPathDirectory(nameFolder);
    } else if (nameLocation == "USB") {
        listPath = folderManager.getListPathUSB(nameFolder);
    }
    
    for (const auto &path : folderManager.getListPathDirectory(nameFolder)) {
        std::cout << "Reading file: " << path << '\n';

        size_t lastSlashPos = path.find_last_of("/");
        std::string fileName = (lastSlashPos != std::string::npos) ? path.substr(lastSlashPos + 1) : "";

        size_t lastDotPos = path.find_last_of(".");
        std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

        if (extension == "mp4") {
            if(this->mediaFileManager.addMediaFile(path, "Video")){
                listPathsAdded.insert(path);
            }
        } else if (extension == "mp3") {
            if(this->mediaFileManager.addMediaFile(path, "Audio")){
                listPathsAdded.insert(path);
            }
        } else {
            std::cerr << "Unsupported file type: " << fileName << '\n';
        }
    }
}

bool fileExists(const std::string& path) {
    return access(path.c_str(), F_OK) == 0; 
}

void MediaScannerController::loadData() 
{
    std::ifstream videoFile("database/video/video.data");
    std::ofstream tempVideoFile("database/video/temp_video.data", std::ios::trunc);

    if (videoFile.is_open() && tempVideoFile.is_open()) {
        std::string line;
        while (std::getline(videoFile, line)) {
            if (fileExists(line)) {
                this->mediaFileManager.loadMediaFile(line, "Video");
                tempVideoFile << line << std::endl; 
            } else {
                std::cerr << "Warning: File does not exist: " << line << std::endl;
            }
        }
        videoFile.close();
        tempVideoFile.close();

        std::remove("database/video/video.data"); 
        std::rename("database/video/temp_video.data", "database/video/video.data");
    }

    std::ifstream audioFile("database/audio/audio.data");
    std::ofstream tempAudioFile("database/audio/temp_audio.data", std::ios::trunc);

    if (audioFile.is_open() && tempAudioFile.is_open()) {
        std::string line;
        while (std::getline(audioFile, line)) {
            if (fileExists(line)) {
                this->mediaFileManager.loadMediaFile(line, "Audio");
                tempAudioFile << line << std::endl; 
            } else {
                std::cerr << "Warning: File does not exist: " << line << std::endl;
            }
        }
        audioFile.close();
        tempAudioFile.close();

        std::remove("database/audio/audio.data"); 
        std::rename("database/audio/temp_audio.data", "database/audio/audio.data");
    }
}



void MediaScannerController::handleScan(bool isRunning)
{
    int choice;
    if((folderManager.getListFolderDirectory().empty() && folderManager.getListFolderUSB().empty()) || isRunning){
        while(1){
            scanView->setListPathNameIsAdded(listPathsAdded);
            listPathsAdded.clear();
            scanView->showMenu();
            std::cin >> choice;
            switch (choice)
            {
            case ScanHomeDirectory:
                scanHomeDirectory();
                break;
            case ScanUSBDevices:
                scanUSBDevices();
                break;
            case Exit:
                return;
            default:
                break;
            }
        }
    }else{
        loadData();
    }
}


std::unordered_set<std::string> MediaScannerController::getListPaths(){return this->listPaths;}

void MediaScannerController::scanHomeDirectory() 
{
    std::unordered_set<std::string> listPathToAdd;

    const char *home = std::getenv("HOME");
    if (!home) {
        std::cerr << "Unable to determine HOME directory.\n";
        return;
    }

    std::string home_path = std::string(home);
    std::vector<std::string> folders = list_folders(home_path);

    if (folders.empty()) {
        std::cerr << "No folders found in Home directory.\n";
        return;
    }

    std::cout << "Folders in Home:\n";
    for (size_t i = 0; i < folders.size(); ++i) {
        std::cout << i + 1 << ". " << folders[i] << '\n';
    }

    int choice = 0;
    std::cout << "Enter the number of the folder you want to scan: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(folders.size())) {
        std::cerr << "Invalid choice.\n";
        return;
    }

    std::string selected_folder = folders[choice - 1];
    std::vector<std::string> media_files = list_media_files(selected_folder);
    
    if(folderManager.getFolderDirectory(selected_folder) != "Folder not found."){
        folderManager.updateFolderDirectory(selected_folder);
    }else{
        folderManager.saveFolderDirectory(selected_folder);
    }

    for (const auto &file : media_files) {
        listPathToAdd.insert(file);
        listPaths.insert(file);
        std::cout << "Reading file in folder"<< selected_folder <<" : " << file << '\n';
    }

    if (listPathToAdd.empty()) {
        std::cerr << "No media files found in the selected folder.\n";
    }else{
        folderManager.addDataFolderDirectory(selected_folder, listPathToAdd);
        addDataFileWithFolder(selected_folder, "Directory");
    }
}

void MediaScannerController::scanUSBDevices() 
{
    std::unordered_set<std::string> listPathToAdd;

    std::string usb_base_path = "/media/" + std::string(std::getenv("USER"));
    std::cout << "Scanning USB devices at: " << usb_base_path << std::endl;

    std::vector<std::string> usb_devices = list_folders(usb_base_path);

    if (usb_devices.empty()) {
        std::cerr << "No USB devices found.\n";
        return;
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
        std::cerr << "Invalid choice.\n";
        return;
    }

    std::string selected_usb = usb_devices[choice - 1];
    std::vector<std::string> folders = list_folders(selected_usb);

    if(folderManager.getFolderUSB(selected_usb) != "Folder not found."){
        folderManager.updateFolderUSB(selected_usb);
    }else{
        folderManager.saveFolderUSB(selected_usb);
    }

    for (const auto &folder : folders) {
        std::vector<std::string> files_in_folder = list_media_files(folder);
        for (const auto &file : files_in_folder) {
            listPathToAdd.insert(file);
            listPaths.insert(file);
            std::cout << "Reading file in folder"<< selected_usb <<" : " << file << '\n';
        }
    }

    if (listPaths.empty()) {
        std::cerr << "No media files found on the selected USB device.\n";
    }else{
        folderManager.addDataFolderUSB(selected_usb, listPathToAdd);
        addDataFileWithFolder(selected_usb , "USB");
    }
}

bool MediaScannerController::checkFolderDirectory(){ return this->folderManager.getListFolderDirectory().empty();}

bool MediaScannerController::checkFolderUSB(){ return this->folderManager.getListFolderUSB().empty();}

std::unordered_set<std::string> MediaScannerController::scanFolder(const std::string &path)
{
    std::unordered_set<std::string> mediaFiles;
    std::vector<std::string> files = list_media_files(path);
    for (const auto &file : files) {
        mediaFiles.insert(file);
    }
    return mediaFiles;
}