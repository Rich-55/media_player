#include "../../include/Controller/MediaScannerController.h"

MediaScannerController::MediaScannerController(MediaFileManager& m, PlaylistManager& p, FolderManager& f,std::shared_ptr<BaseView> v)
     : mediaFileManager(m), playlistManager(p) ,folderManager(f), scanView(v){}

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

void MediaScannerController::addDataFileWithFolder(std::string nameFolder, std::string nameLocation) 
{
    std::unordered_set<std::string> listPath;

    if (nameLocation == "Directory") {
        listPath = folderManager.getListPathDirectory(nameFolder);
    } else if (nameLocation == "USB") {
        listPath = folderManager.getListPathUSB(nameFolder);
    }
    
    for (const auto &path : folderManager.getListPathDirectory(nameFolder)) {

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

bool fileExists(const std::string& path) {return access(path.c_str(), F_OK) == 0; }

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

void MediaScannerController::loadFolder()
{
    try {
        // Load folders from database/listFolderDirectory.data
        std::ifstream dirFile("database/listFolderDirectory.data");
        if (!dirFile.is_open()) {
            throw std::runtime_error("Could not open listFolderDirectory.data");
        }

        std::string folderPath;
        while (std::getline(dirFile, folderPath)) {
            if (folderPath.empty()) {
                continue;
            }

            std::unordered_set<std::string> mediaFiles = scanFolder(folderPath);
            if (!mediaFiles.empty()) {
                folderManager.addDataFolderDirectory(folderPath, mediaFiles);
            }
        }
        dirFile.close();

        // Load folders from database/listFolderUSB.data
        std::ifstream usbFile("database/listFolderUSB.data");
        if (!usbFile.is_open()) {
            throw std::runtime_error("Could not open listFolderUSB.data");
        }

        while (std::getline(usbFile, folderPath)) {
            if (folderPath.empty()) {
                continue;
            }

            std::unordered_set<std::string> mediaFiles = scanFolder(folderPath);
            if (!mediaFiles.empty()) {
                folderManager.addDataFolderUSB(folderPath, mediaFiles);
            }
        }
        usbFile.close();

    } catch (const std::exception &e) {
        std::cerr << "Error in loadFolder: " << e.what() << std::endl;
    }
}

void MediaScannerController::loadMediaPlaylist()
{
    std::string directory = "database/playlist";

    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        std::cerr << "Playlist directory does not exist or is not a directory: " << directory << "\n";
        return;
    }

    std::vector<std::shared_ptr<MediaFile>> allMediaFiles = mediaFileManager.getAllMediaFile();

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) { 
            std::string fileName = entry.path().filename().string();
            std::string playlistName = fileName.substr(0, fileName.find_last_of('.')); 

            std::shared_ptr<Playlist> playlist = std::make_shared<Playlist>(playlistName);

            std::ifstream inFile(entry.path());
            if (inFile.is_open()) {
                std::string mediaPath;
                while (std::getline(inFile, mediaPath)) {
                    for (const auto &mediaFile : allMediaFiles) {
                        if (mediaFile->getPath() == mediaPath) {
                            playlist->loadMediaFile(mediaFile);
                            break;
                        }
                    }
                }
                inFile.close();
            } else {
                std::cerr << "Failed to open playlist file: " << entry.path() << "\n";
            }

            playlistManager.loadPlaylist(playlist);
        }
    }
}

std::unordered_set<std::string> MediaScannerController::getListPaths(){return this->listPaths;}

void MediaScannerController::scanHomeDirectory() 
{
    try{
        int choice;
        std::unordered_set<std::string> listPathToAdd;
        std::vector<std::string> folders;

        const char *home = std::getenv("HOME");
        if (!home) {
            throw HomeDirectoryException(); 
        }

        std::string home_path = std::string(home);
        folders = list_folders(home_path);

        if (folders.empty()) {
            throw NoFoldersFoundException(home_path);
        }

        choice = scanView->showListFolder(folders);
        if(choice == -1){
            return;
        }
        std::string selected_folder = folders[choice];
        std::vector<std::string> media_files = list_media_files(selected_folder);
        
        if(folderManager.getFolderDirectory(selected_folder) != "Folder not found."){
            folderManager.updateFolderDirectory(selected_folder);
        }else{
            folderManager.saveFolderDirectory(selected_folder);
        }

        for (const auto &file : media_files) {
            listPathToAdd.insert(file);
            listPaths.insert(file);
        }

        if (listPathToAdd.empty()) {
            throw NoMediaFilesFoundException(selected_folder);
        }else{
            folderManager.addDataFolderDirectory(selected_folder, listPathToAdd);
            addDataFileWithFolder(selected_folder, "Directory");
        }
    }catch (const ScanException &e) {
        std::cerr << e.what() << '\n';
    }
}

void MediaScannerController::scanUSBDevices() {
    try {
        std::unordered_set<std::string> listPathToAdd;
        int choiceUSB;
        int choiceFolder;

        std::string usb_base_path = "/media/" + std::string(std::getenv("USER"));
        std::cout << "Scanning USB devices at: " << usb_base_path << std::endl;

        std::vector<std::string> usb_devices = list_folders(usb_base_path);

        if (usb_devices.empty()) {
            throw NoUSBDevicesFoundException();
        }

        choiceUSB = scanView->showListUSBName(usb_devices);
        if(choiceUSB == -1){
            return;
        }
        std::string selected_usb = usb_devices[choiceUSB];
        std::vector<std::string> folders = list_folders(selected_usb);

        if (folders.empty()) {
            throw NoFoldersFoundException(selected_usb);
        }

        choiceFolder = scanView->showListFolder(folders);
        if(choiceFolder == -1){
            return;
        }
        std::string selected_folder = folders[choiceFolder];

        std::vector<std::string> files_in_folder = list_media_files(selected_folder);

        if(folderManager.getFolderUSB(selected_folder) != "Folder not found."){
            folderManager.updateFolderUSB(selected_folder);
        }else{
            folderManager.saveFolderUSB(selected_folder);
        }
        
        for (const auto &file : files_in_folder) {
            listPathToAdd.insert(file);
            listPaths.insert(file);
        }

        if (listPaths.empty()) {
            throw NoMediaFilesFoundException(selected_folder);
        } else {
            folderManager.addDataFolderUSB(selected_folder, listPathToAdd);
            addDataFileWithFolder(selected_folder, "USB");
        }
    } catch (const ScanException &e) {
        std::cerr << e.what() << '\n';
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

void MediaScannerController::handleScan(bool isRunning)
{
    try {
        int choice;
        if ((folderManager.getListFolderDirectory().empty() && folderManager.getListFolderUSB().empty()) || isRunning) {
            while (true) { 
                try {
                    system("clear");
                    scanView->setListPathNameIsAdded(listPathsAdded);
                    listPathsAdded.clear();
                    choice = scanView->showMenu();
                    
                    switch (choice) {
                        case SCAN_HOME_DIRECTORY:
                            scanHomeDirectory();
                            break;
                        case SCAN_USB:
                            scanUSBDevices();
                            break;
                        case EXIT_MENU_SCAN:
                            return; 
                        default:
                            throw InvalidChoiceException(); 
                    }
                }catch (const InvalidChoiceException &e) {
                    std::cerr << "Error: " << e.what() << '\n'; 
                }
            }
        } else {
            loadData();
            loadFolder();
            loadMediaPlaylist();
        }
    } catch (const ScanException &e) {
        std::cerr << e.what() << '\n';
    }
}
