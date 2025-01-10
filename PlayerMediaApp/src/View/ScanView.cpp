#include "../../include/View/ScanView.h"

ScanView::ScanView() {}

bool has_extension(const std::string &filename, const std::string &extension) {
    if (filename.size() >= extension.size()) {
        return (filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0);
    }
    return false;
}

std::vector<std::string> list_folders(const std::string &path) {
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

std::vector<std::string> list_media_files(const std::string &path) {
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

std::vector<std::string> scan_all_folders(const std::string &path) {
    std::vector<std::string> all_media_files;
    std::vector<std::string> folders = list_folders(path);
    for (const auto &folder : folders) {
        std::vector<std::string> media_files = list_media_files(folder);
        all_media_files.insert(all_media_files.end(), media_files.begin(), media_files.end());  
    }
    return all_media_files;
}

void ScanView::scanHomeDirectory(std::vector<std::string>& listPathNames) {
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

    // Hiển thị danh sách các thư mục trong Home và yêu cầu người dùng chọn
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

    // Lấy đường dẫn của thư mục đã chọn và quét các file MP3/MP4 trong thư mục đó
    std::string selected_folder = folders[choice - 1];
    listPathNames = list_media_files(selected_folder);  // Lưu danh sách file vào listPathNames
}

void ScanView::scanUSBDevices(std::vector<std::string>& listPathNames) {
    std::string usb_base_path = "/media/" + std::string(std::getenv("USER"));
    std::vector<std::string> usb_devices = list_folders(usb_base_path);

    if (usb_devices.empty()) {
        std::cerr << "No USB devices found.\n";
        return;
    }

    // Quét tất cả các thư mục trên USB mà không yêu cầu người dùng chọn
    for (const auto& usb : usb_devices) {
        std::vector<std::string> folders = list_folders(usb);
        for (const auto& folder : folders) {
            std::vector<std::string> media_files = list_media_files(folder);
            listPathNames.insert(listPathNames.end(), media_files.begin(), media_files.end());
        }
    }
}
void ScanView::showMenu(){
    std::cout << "Menu Scan View\n";
    std::cout << "1. Scan Home Directory\n";
    std::cout << "2. Scan USB Devices\n";
    std::cout << "Enter your choice: ";
}


