#include "../../include/Model/FolderManager.h"

FolderManager::FolderManager(){}

std::unordered_set<std::string> FolderManager::getListFolderDirectory()
{
    std::ifstream inFile("database/listFolderDirectory.data");
    if (!inFile) {
        std::cerr << "Failed to open file.\n";
        return {};  
    }

    std::string folder;
    while (std::getline(inFile, folder)) {
        listPathFolderDirectory[folder];  
    }
    inFile.close();

    std::unordered_set<std::string> folderKeys;
    for (const auto& entry : listPathFolderDirectory) {
        folderKeys.insert(entry.first);  
    }

    return folderKeys;
}

std::unordered_set<std::string> FolderManager::getListFolderUSB()
{
    std::ifstream inFile("database/listFolderUSB.data");
    if (!inFile) {
        std::cerr << "Failed to open file.\n";
        return {};  
    }

    std::string folder;
    while (std::getline(inFile, folder)) {
        listPathFolderUSB[folder];  
    }
    inFile.close();

    std::unordered_set<std::string> folderKeys;
    for (const auto& entry : listPathFolderUSB) {
        folderKeys.insert(entry.first);  
    }

    return folderKeys;
}

std::unordered_set<std::string> FolderManager::getListPathDirectory(std::string nameFolder)
{
    auto it = listPathFolderDirectory.find(nameFolder);
    if (it != listPathFolderDirectory.end()) {
        return it->second;
    }else{
        return {};
    }
}

std::unordered_set<std::string> FolderManager::getListPathUSB(std::string nameFolder)
{
    auto it = listPathFolderUSB.find(nameFolder);
    if (it != listPathFolderUSB.end()) {
        return it->second;
    }else{
        return {};
    } 
}


std::string FolderManager::getFolderDirectory(std::string folderName)
{
    auto it = listPathFolderDirectory.find(folderName);
    if (it != listPathFolderDirectory.end()) {
        return it->first;
    } else {
        return "Folder not found.";
    }
}
std::string FolderManager::getFolderUSB(std::string folderName)
{
    auto it = listPathFolderUSB.find(folderName);
    if (it != listPathFolderUSB.end()) {
        return it->first;
    } else {
        return "Folder not found.";
    }
}

void FolderManager::addDataFolderDirectory(const std::string &folder, std::unordered_set<std::string> listFiles)
{
    listPathFolderDirectory[folder] = listFiles;
}

void FolderManager::saveFolderDirectory(std::string folderName)
{  
    try {
        std::ofstream outFile("database/listFolderDirectory.data", std::ios::app);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file.");
        }
        outFile << folderName << '\n';
        outFile.close(); 
    } catch (const std::exception &e) {
        std::cerr << "Error writing to file: " << e.what() << '\n';
    }
}


void FolderManager::addDataFolderUSB(const std::string &folder, std::unordered_set<std::string> listFiles) 
{
    listPathFolderUSB[folder] = listFiles;  
}

void FolderManager::saveFolderUSB(std::string folderName)
{
    try {
        std::ofstream outFile("database/listFolderUSB.data", std::ios::app);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file.");
        }
        outFile << folderName << '\n';
        outFile.close(); 
    } catch (const std::exception &e) {
        std::cerr << "Error writing to file: " << e.what() << '\n';
    }
}

void FolderManager::updateFolderDirectory(std::string folderName)
{
    auto it = listPathFolderDirectory.find(folderName);

    if (it != listPathFolderDirectory.end()) {
        it->second.clear();
    } else {
        std::cout << "Folder not found: " << folderName << std::endl;
    }
}

void FolderManager::updateFolderUSB(std::string folderName)
{
    auto it = listPathFolderUSB.find(folderName);

    if (it != listPathFolderUSB.end()) {
        it->second.clear();
    } else {
        std::cout << "Folder not found: " << folderName << std::endl;
    }
}
