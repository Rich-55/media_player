#include "../../include/View/PlaylistHandlerView.h"

PlaylistHandlerView::PlaylistHandlerView(){}
        
int PlaylistHandlerView::showMenu(){
    std::cout << "============Playlist Manager==========" << std::endl;
    std::cout << "1. Add MediaFile" << std::endl;
    std::cout << "2. Delete MediaFile" << std::endl;
    std::cout << "3. Display All MediaFiles" << std::endl;
    std::cout << "0. Exit" << std::endl;
    return 1;
}

void PlaylistHandlerView::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist> playlist)
{
    std::cout << "============Display All Media File:" << playlist->getName() << "==========" << std::endl;
    if(playlist->getListMediaFiles().empty()){
        std::cerr << "No media files in the playlist.\n";
        return;
    }
    for(auto mediaFile : playlist->getListMediaFiles()){
        std::cout << mediaFile->getName() << std::endl;
    }
}
