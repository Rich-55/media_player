#include "../../include/Model/PlaylistManager.h"

PlaylistManager::PlaylistManager(){}

bool PlaylistManager::checkPlaylist(){
    if(listPlaylist.empty()){
        return false;
    }
    return true;
}

// Định nghĩa hàm playlistExists
bool PlaylistManager::playlistExists(const std::string& name) const {
    for (const auto& playlist : listPlaylist) {
        if (playlist->getName() == name) {
            return true;
        }
    }
    return false;
}

bool PlaylistManager::mediaFileExists(const std::string& mediaFileName) const {
    for (const auto& playlist : listPlaylist) {
        if (playlist->getName() == mediaFileName) {
            return true;
        }
    }
    return false;  // Media file không tồn tại
}


bool PlaylistManager::checkPlaylistName(std::string name){
    for(const auto &playlist : listPlaylist){
        if(playlist->getName() == name){
            return true;
        }
    }
    return false;
}

std::shared_ptr<Playlist> PlaylistManager::getPlaylist(std::string name) {
    for (auto playlist : listPlaylist) {
        if (playlist->getName() == name) {
            return playlist;
        }
    }
    std::cerr << "Playlist does not exist.\n";
    return nullptr; 
}


void PlaylistManager::addPlaylist(std::shared_ptr<Playlist>  playlist){
    for(const auto &p : listPlaylist){
        if(p->getName() == playlist->getName()){
            std::cerr << "Playlist already exists.\n";
            return;
        }
    }

    listPlaylist.push_back(playlist);
    std::cout << "Playlist created successfully.\n";
}

void PlaylistManager::deletePlaylist(std::string name){
    if(!checkPlaylistName(name)){
        std::cerr << "Playlist does not exist.\n";
        return;
    }
    for(auto it = listPlaylist.begin(); it != listPlaylist.end(); ++it){
        if((*it)->getName() == name){
            listPlaylist.erase(it);
            std::cout << "Playlist deleted successfully.\n";
            return;
        }
    }
}


std::vector<std::shared_ptr<Playlist> > PlaylistManager::getAllPlaylist(){
    return listPlaylist;
}

PlaylistManager::~PlaylistManager(){
    listPlaylist.clear();
}