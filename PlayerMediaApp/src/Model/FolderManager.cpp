#include "../../include/Model/FolerManager.h"

FolderManager::FolderManager(){}

std::unordered_set<std::string> FolderManager::getListFolderDirectory(){
    //read from file
    std::ifstream inFile("database/listFolderDirectory.data");
    if (!inFile) {
        std::cerr << "Failed to open file.\n";
        return {};
    }

    std::string folder;
    while (std::getline(inFile, folder)) {
        listFolderDirectory.insert(folder);
    }
    inFile.close();

    return listFolderDirectory;
}

std::unordered_set<std::string> FolderManager::getListFolderUSB(){
    //read from file
    std::ifstream inFile("database/listFolderUSB.data");
    if (!inFile) {
        std::cerr << "Failed to open file.\n";
        return {};
    }

    std::string folder;
    while (std::getline(inFile, folder)) {
        listFolderUSB.insert(folder);
    }
    inFile.close();

    return listFolderUSB;
}

void FolderManager::addDataFolderDirectory(const std::string &folder) {
    for (const auto &f : listFolderDirectory) {
        if (f == folder) {
            std::cerr << "Folder already exists.\n";
            return;
        }
    }

    listFolderDirectory.insert(folder);
    std::cout << "Folder added successfully.\n";

    try {
        std::ofstream outFile("database/listFolderDirectory.data", std::ios::app);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file.");
        }
        outFile << folder << '\n';
        outFile.close(); 
    } catch (const std::exception &e) {
        std::cerr << "Error writing to file: " << e.what() << '\n';
    }
}

void FolderManager::addDataFolderUSB(const std::string &folder) {
    for (const auto &f : listFolderUSB) {
        if (f == folder) {
            std::cerr << "Folder already exists.\n";
            return;
        }
    }

    listFolderUSB.insert(folder);
    std::cout << "Folder added successfully.\n";

    try {
        std::ofstream outFile("database/listFolderUSB.data", std::ios::app);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file.");
        }
        outFile << folder << '\n';
        outFile.close(); 
    } catch (const std::exception &e) {
        std::cerr << "Error writing to file: " << e.what() << '\n';
    }
}