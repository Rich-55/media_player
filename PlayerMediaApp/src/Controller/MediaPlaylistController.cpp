#include "../../include/Controller/MediaPlaylistController.h"

MediaPlaylistController::MediaPlaylistController(MediaFileManager& mediaFileManager, FolderManager& folderManager, std::shared_ptr<Playlist> playlistModel, std::shared_ptr<BaseView> mediaManagerView, std::shared_ptr<BaseView> playlistHandlerView) 
: mediaFileManager(mediaFileManager), folderManager(folderManager), playlist(playlistModel), mediaManagerView(mediaManagerView), playlistHandlerView(playlistHandlerView)
{}

std::vector<std::string> MediaPlaylistController::getListPathMediaFiles() {
    return playlist->getListPathMediaFiles();
}

void MediaPlaylistController::setNamePlaylist(std::string name) {
    playlist->setName(name);
}

bool MediaPlaylistController::addMediaFileInPlaylist(std::string fileName) {
    std::shared_ptr<MediaFile> mediaFile = mediaFileManager.getMediaFile(fileName);
    if (playlist->checkMediaFile(mediaFile->getName())) {
        return false;
    }
    playlist->addMediaFile(mediaFile);
    return true;  
}

bool MediaPlaylistController::addMediaFileByFolder()
{
    bool check = false;

    // Lấy danh sách folder từ Directory và USB
    std::unordered_set<std::string> listFolderOfDirectory = folderManager.getListFolderDirectory();
    std::unordered_set<std::string> listFolderOfUSB = folderManager.getListFolderUSB();

    std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>> listFolder;
    listFolder.first = listFolderOfDirectory;
    listFolder.second = listFolderOfUSB;

    // Hiển thị tất cả folder cho người dùng chọn
    std::pair<std::string, std::string> choice = playlistHandlerView->displayAllFolder(listFolder);

    if (choice.first.empty() || choice.second.empty()) {
        return false;
    }

    // Kiểm tra loại folder (Directory hoặc USB)
    if (choice.first == "Directory") {
        std::unordered_set<std::string> listPathDirectory = folderManager.getListPathDirectory(choice.second);

        for (const auto &list : listPathDirectory) {
            std::shared_ptr<MediaFile> mediaFile = mediaFileManager.getMediaFileByPath(list);

            // Kiểm tra nếu con trỏ mediaFile là nullptr
            if (!mediaFile) {
                std::cerr << "Warning: Media file not found for path: " << list << std::endl;
                continue;
            }

            // Kiểm tra nếu file đã tồn tại trong playlist
            if (playlist->checkMediaFile(mediaFile->getName())) {
                continue;
            }

            playlist->addMediaFile(mediaFile);
            check = true;
        }
    } else if (choice.first == "USB") {
        std::unordered_set<std::string> listPathUSB = folderManager.getListPathUSB(choice.second);

        for (const auto &list : listPathUSB) {
            std::shared_ptr<MediaFile> mediaFile = mediaFileManager.getMediaFileByPath(list);

            // Kiểm tra nếu con trỏ mediaFile là nullptr
            if (!mediaFile) {
                std::cerr << "Warning: Media file not found for path: " << list << std::endl;
                continue;
            }

            // Kiểm tra nếu file đã tồn tại trong playlist
            if (playlist->checkMediaFile(mediaFile->getName())) {
                continue;
            }

            playlist->addMediaFile(mediaFile);
            check = true;
        }
    }

    return check;
} 


bool MediaPlaylistController::deleteMediaFileInPlaylist(std::string fileName) {
    
    bool check = false;
    
    if (!playlist->checkMediaFile(fileName)) {
        check = false;
    }

    if(playlistHandlerView->showConfirmMenu("Do you want to delete this file: " + fileName + " ?")){
        playlist->deleteMediaFile(fileName);
        check = true;
    }

    return check;
}


void MediaPlaylistController::displayAllMediaFilesInPlaylist()
{
    playlistHandlerView->displayAllMediaFileInPlaylist(playlist);
}

int MediaPlaylistController::showMenuWithMediaListInPlaylist()
{
    return playlistHandlerView->showMenuWithMediaListInPlaylist(playlist);
}

void MediaPlaylistController::handlerPlaylist() {
    int choice;
    std::string message;
    std::string error;
    while (true) {
        system("clear");
        try {
            
            if(!message.empty()){
                playlistHandlerView->showNotificationMessage(message, "success");
                message = "";
            }

            if(!error.empty()){
                playlistHandlerView->showNotificationMessage(error, "error");
                error = "";
            }
            
            choice = showMenuWithMediaListInPlaylist();
           
            switch (choice) {
            case ADD_MEDIA_FILE_TO_PLAYLIST:
            {
                std::string fileName;

                fileName = mediaManagerView->displayAllMediaFile(mediaFileManager);

                if(fileName == "exit"){
                    break;
                }

                if(addMediaFileInPlaylist(fileName))
                {
                    message = "File " + fileName + " has been added to the playlist.";
                }else{
                    error = "File " + fileName + " is already in the playlist.";
                }
                break;
            } 
            case ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH:
            {
                if(addMediaFileByFolder()){
                    message = "All files in the folder have been added to the playlist.";
                }else{
                    error = "No files in the folder have been added to the playlist.";
                }
                break;
            }
            case DELETE_MEDIA_FILE_FROM_PLAYLIST:
            {   while(true){
                    std::string fileName;
                    fileName = playlistHandlerView->displayAllMediaFileInPlaylist(playlist);
                    if(fileName == "exit"){
                        break;
                    }
                    if(deleteMediaFileInPlaylist(fileName)){
                        message = "File " + fileName + " has been deleted from the playlist.";
                    }else{
                        continue;
                    }
                    break;
                }
            }   
                break;
            case RENAME_PLAYLIST:
            {
                std::string newPlaylistName = playlistHandlerView->showMenuCreatePlaylist();
                if (newPlaylistName == "exit") {
                    break;
                }
                playlist->setName(newPlaylistName);
                message = "Playlist has been renamed to " + newPlaylistName;
                break;
            }
            case EXIT_MENU_PLAYLIST_HANDLER:
                return;
            default:
                throw InvalidChoiceException();
            }
        } catch (const PlaylistHandlerException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }catch (const InvalidChoiceException &e) {
            std::cerr << "Error: " << e.what() << '\n'; 
        }
    }
}
