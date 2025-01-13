#ifndef FOLDER_MANAGER_H
#define FOLDER_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>
class FolderManager{
    private:
        std::unordered_set<std::string> listFolderDirectory;
        std::unordered_set<std::string> listFolderUSB;
    public:
        FolderManager();

        std::unordered_set<std::string> getListFolderDirectory();
        std::unordered_set<std::string> getListFolderUSB();

        void addDataFolderDirectory(const std::string &);
        void addDataFolderUSB(const std::string &);

        void deleteFolder(std::string);
};
#endif