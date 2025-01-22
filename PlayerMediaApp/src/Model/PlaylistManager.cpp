#include "../../include/Model/PlaylistManager.h"

PlaylistManager::PlaylistManager(){}

bool PlaylistManager::checkPlaylist()
{
    if(listPlaylist.empty()){
        return false;
    }
    return true;
}

bool PlaylistManager::checkPlaylistName(std::string name)
{
    for(const auto &playlist : listPlaylist){
        if(playlist->getName() == name){
            return true;
        }
    }
    return false;
}

std::shared_ptr<Playlist> PlaylistManager::getPlaylist(std::string name) 
{
    for (auto playlist : listPlaylist) {
        if (playlist->getName() == name) {
            return playlist;
        }
    }
    std::cerr << "Playlist does not exist.\n";
    return nullptr; 
}

void PlaylistManager::loadPlaylist(std::shared_ptr<Playlist> playlist)
{
    for (const auto &p : listPlaylist) {
        if (p->getName() == playlist->getName()) {
            std::cerr << "Playlist already exists.\n";
            return;
        }
    }
    listPlaylist.push_back(playlist);
}


void PlaylistManager::addPlaylist(std::shared_ptr<Playlist> playlist) 
{
    for (const auto &p : listPlaylist) {
        if (p->getName() == playlist->getName()) {
            std::cerr << "Playlist already exists.\n";
            return;
        }
    }

    listPlaylist.push_back(playlist);

    std::string directory = "database/playlist";
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directories(directory);
    }

    std::string filePath = directory + "/" + playlist->getName() + ".playlist";
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile.close();
        std::cout << "Playlist created successfully and saved as " << filePath << ".\n";
    } else {
        std::cerr << "Failed to create playlist file: " << filePath << ".\n";
    }
}

bool PlaylistManager::deletePlaylist(std::string name)
{
    if (!checkPlaylistName(name)) {
        std::cerr << "Playlist does not exist.\n";
        return false;
    }

    std::string directory = "database/playlist";
    std::string playlistFilePath = directory + "/" + name + ".playlist";

    if (std::filesystem::exists(playlistFilePath)) {
        if (std::filesystem::remove(playlistFilePath)) {
            std::cout << "Playlist file deleted: " << playlistFilePath << "\n";
        } else {
            std::cerr << "Failed to delete playlist file: " << playlistFilePath << "\n";
            return false;
        }
    } else {
        std::cerr << "Playlist file does not exist in database: " << playlistFilePath << "\n";
    }

    for (auto it = listPlaylist.begin(); it != listPlaylist.end(); ++it) {
        if ((*it)->getName() == name) {
            listPlaylist.erase(it);
            std::cout << "Playlist deleted successfully.\n";
            return true;
        }
    }
    return false;
}


std::vector<std::shared_ptr<Playlist> > PlaylistManager::getAllPlaylist(){return listPlaylist;}

PlaylistManager::~PlaylistManager(){ listPlaylist.clear();}