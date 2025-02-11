#ifndef FOLDER_MANAGER_H
#define FOLDER_MANAGER_H

#include "../utils/ConfigLoader.h"

class FolderManager{
    private:
        std::unordered_map<std::string, std::unordered_set<std::string>> listPathFolderDirectory;
        std::unordered_map<std::string, std::unordered_set<std::string>> listPathFolderUSB;
    public:
        FolderManager();
        virtual std::unordered_set<std::string> getListFolderDirectory();
        virtual std::unordered_set<std::string> getListFolderUSB();
        
        virtual std::unordered_set<std::string> getListPathDirectory(std::string nameFolder);
        virtual std::unordered_set<std::string> getListPathUSB(std::string nameFolder);

        virtual std::string getFolderDirectory(std::string folderName);
        virtual std::string getFolderUSB(std::string folderName);

        virtual void saveFolderDirectory(std::string folderName);
        virtual void saveFolderUSB(std::string folderName);

        virtual void updateFolderDirectory(std::string folderName);
        virtual void updateFolderUSB(std::string folderName);

        virtual void addDataFolderDirectory(const std::string &folder, std::unordered_set<std::string> listFiles);
        virtual void addDataFolderUSB(const std::string &folder, std::unordered_set<std::string> listFiles);
};
#endif
