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
        listFolderDirectory[folder];  
    }
    inFile.close();

    std::unordered_set<std::string> folderKeys;
    for (const auto& entry : listFolderDirectory) {
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
        listFolderUSB[folder];  
    }
    inFile.close();

    std::unordered_set<std::string> folderKeys;
    for (const auto& entry : listFolderUSB) {
        folderKeys.insert(entry.first);  
    }

    return folderKeys;
}

std::unordered_set<std::string> FolderManager::getListPathDirectory(std::string nameFolder)
{
    auto it = listFolderDirectory.find(nameFolder);
    if (it != listFolderDirectory.end()) {
        return it->second;
    }else{
        return {};
    }
}

std::unordered_set<std::string> FolderManager::getListPathUSB(std::string nameFolder)
{
    auto it = listFolderUSB.find(nameFolder);
    if (it != listFolderUSB.end()) {
        return it->second;
    }else{
        return {};
    } 
}


std::string FolderManager::getFolderDirectory(std::string folderName)
{
    auto it = listFolderDirectory.find(folderName);
    if (it != listFolderDirectory.end()) {
        return it->first;
    } else {
        return "Folder not found.";
    }
}
std::string FolderManager::getFolderUSB(std::string folderName)
{
    auto it = listFolderUSB.find(folderName);
    if (it != listFolderUSB.end()) {
        return it->first;
    } else {
        return "Folder not found.";
    }
}

void FolderManager::addDataFolderDirectory(const std::string &folder, std::unordered_set<std::string> listFiles)
{
    listFolderDirectory[folder] = listFiles;
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
    listFolderDirectory[folder] = listFiles;  
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
    auto it = listFolderDirectory.find(folderName);

    if (it != listFolderDirectory.end()) {
        it->second.clear();
    } else {
        std::cout << "Folder not found: " << folderName << std::endl;
    }
}

void FolderManager::updateFolderUSB(std::string folderName)
{
    auto it = listFolderUSB.find(folderName);

    if (it != listFolderUSB.end()) {
        it->second.clear();
    } else {
        std::cout << "Folder not found: " << folderName << std::endl;
    }
}
