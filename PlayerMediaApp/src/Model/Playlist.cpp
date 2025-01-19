#include "../../include/Model/Playlist.h"

Playlist::Playlist(std::string name) : name(name) {}

std::string Playlist::getName() const
{
    return this->name;
}

bool Playlist::checkMediaFile(std::string file)
{
    for(const auto &mediaFile : listMediaFiles)
    {
        if(mediaFile->getName() == file)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<MediaFile>> Playlist::getListMediaFiles()
{
    return this->listMediaFiles;
}

std::vector<std::string> Playlist::getListPathMediaFiles()
{
    std::vector<std::string> listPathMediaFiles;
    for(const auto &mediaFile : listMediaFiles)
    {
        listPathMediaFiles.push_back(mediaFile->getPath());
    }
    return listPathMediaFiles;
}

void Playlist::loadMediaFile(std::shared_ptr<MediaFile> file)
{
    listMediaFiles.push_back(file);
}

void Playlist::addMediaFile(std::shared_ptr<MediaFile> file) 
{   
    std::cout << "Playlist name: " << name << std::endl;

    for (const auto &mediaFile : listMediaFiles)
    {
        if (mediaFile->getName() == file->getName())
        {   
            if(mediaFile->getSize() == file->getSize())
            {
                std::cerr << "File already exists in the playlist.\n";
                return;
            }
        }
    }

    listMediaFiles.push_back(file);
    std::cout << "Media file added to playlist.\n";

    std::string directory = "database/playlist";
    std::string playlistFilePath = directory + "/" + name + ".playlist";

    if (std::filesystem::exists(playlistFilePath))
    {
        std::ofstream outFile(playlistFilePath, std::ios::app); 
        if (outFile.is_open())
        {
            outFile << file->getPath() << "\n"; 
            std::cout << "Media file path added to playlist file: " << playlistFilePath << "\n";
        }
        else
        {
            std::cerr << "Failed to open playlist file: " << playlistFilePath << "\n";
        }
    }
    else
    {
        std::cerr << "Playlist file does not exist: " << playlistFilePath << "\n";
    }
}

void Playlist::deleteMediaFile(std::string fileName) {

    std::shared_ptr<MediaFile> targetMedia = nullptr;
    for (const auto& mediaFile : listMediaFiles) {
        if (mediaFile->getName() == fileName) {
            targetMedia = mediaFile;
            break;
        }
    }

    if (!targetMedia) {
        std::cerr << "File not found in the playlist.\n";
        return;
    }

    std::string filePath = targetMedia->getPath(); 
    std::string directory = "database/playlist";
    std::string playlistFilePath = directory + "/" + name + ".playlist";

    if (!std::filesystem::exists(playlistFilePath)) {
        std::cerr << "Playlist file does not exist: " << playlistFilePath << "\n";
        return;
    }

    listMediaFiles.erase(std::remove(listMediaFiles.begin(), listMediaFiles.end(), targetMedia), listMediaFiles.end());

    std::ifstream inFile(playlistFilePath);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open playlist file: " << playlistFilePath << "\n";
        return;
    }

    std::vector<std::string> remainingPaths;
    std::string line;
    while (std::getline(inFile, line)) {
        if (line != filePath) {
            remainingPaths.push_back(line); 
        }
    }
    inFile.close();

    std::ofstream outFile(playlistFilePath, std::ios::trunc); 
    if (outFile.is_open()) {
        for (const auto& path : remainingPaths) {
            outFile << path << "\n";
        }
        outFile.close();
        std::cout << "Media file deleted and playlist file updated: " << playlistFilePath << "\n";
    } else {
        std::cerr << "Failed to write to playlist file: " << playlistFilePath << "\n";
    }
}


Playlist::~Playlist() {}