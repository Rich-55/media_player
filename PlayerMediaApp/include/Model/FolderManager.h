#ifndef FOLDER_MANAGER_H
#define FOLDER_MANAGER_H

#include "../utils/ConfigLoader.h"

class FolderManager{
    private:
        std::unordered_map<std::string, std::unordered_set<std::string>> listFolderDirectory;
        std::unordered_map<std::string, std::unordered_set<std::string>> listFolderUSB;
    public:
        FolderManager();

        std::unordered_set<std::string> getListFolderDirectory();
        std::unordered_set<std::string> getListFolderUSB();
        
        std::unordered_set<std::string> getListPathDirectory(std::string nameFolder);
        std::unordered_set<std::string> getListPathUSB(std::string nameFolder);

        std::string getFolderDirectory(std::string folderName);
        std::string getFolderUSB(std::string folderName);

        void saveFolderDirectory(std::string folderName);
        void saveFolderUSB(std::string folderName);

        void updateFolderDirectory(std::string folderName);
        void updateFolderUSB(std::string folderName);

        void addDataFolderDirectory(const std::string &folder, std::unordered_set<std::string> listFiles);
        void addDataFolderUSB(const std::string &folder, std::unordered_set<std::string> listFiles);
};
#endif
