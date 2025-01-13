#include "../../include/View/PlaylistHandlerView.h"

PlaylistHandlerView::PlaylistHandlerView(){}
        
void PlaylistHandlerView::showMenu(){
    std::cout << "============Playlist Manager==========" << std::endl;
    std::cout << "1. Add MediaFile" << std::endl;
    std::cout << "2. Delete MediaFile" << std::endl;
    std::cout << "3. Display All MediaFiles" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void PlaylistHandlerView::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist> playlist)
{
    std::cout << "============Display All Media File:" << playlist->getName() << "==========" << std::endl;
    for(auto mediaFile : playlist->getListMediaFiles()){
        std::cout << mediaFile->getName() << std::endl;
    }
}
