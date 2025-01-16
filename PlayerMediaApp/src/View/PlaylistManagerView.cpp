#include "../../include/View/PlaylistManagerView.h"

PlaylistManagerView::PlaylistManagerView(){}
        
int PlaylistManagerView::showMenu(){
    std::cout << "============Playlist Manager==========" << std::endl;
    std::cout << "1. Create new playlist" << std::endl;
    std::cout << "2. Delete media file from playlist" << std::endl;
    std::cout << "3. Show all playlist" << std::endl;
    std::cout << "0. Exit" << std::endl;
    return 1;
}

void PlaylistManagerView::displayAllPlaylist(PlaylistManager playlistManager){
    if(!playlistManager.checkPlaylist()){
        std::cerr << "Playlist is empty.\n";
        return;
    }
    std::cout << "============All Playlist==========" << std::endl;
    for(auto playlist : playlistManager.getAllPlaylist()){
        std::cout << playlist->getName() << std::endl;
        std::cout << std::string(50, '-') << std::endl;
    }
}