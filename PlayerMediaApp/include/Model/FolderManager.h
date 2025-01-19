#ifndef FOLDER_MANAGER_H
#define FOLDER_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
class FolderManager{
    private:
        std::unordered_map<std::string, std::unordered_set<std::string>> listFolderDirectory;
        std::unordered_map<std::string, std::unordered_set<std::string>> listFolderUSB;
    public:
        FolderManager();

        std::unordered_set<std::string> getListFolderDirectory();
        std::unordered_set<std::string> getListFolderUSB();
        
        std::unordered_set<std::string> getListPathDirectory(std::string);
        std::unordered_set<std::string> getListPathUSB(std::string);

        std::string getFolderDirectory(std::string);
        std::string getFolderUSB(std::string);

        void saveFolderDirectory(std::string);
        void saveFolderUSB(std::string);

        void updateFolderDirectory(std::string);
        void updateFolderUSB(std::string);

        void addDataFolderDirectory(const std::string &, std::unordered_set<std::string> );
        void addDataFolderUSB(const std::string &, std::unordered_set<std::string> );

        void deleteFolder(std::string);
};
#endif