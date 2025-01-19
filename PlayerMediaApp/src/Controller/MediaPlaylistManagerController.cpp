#include "../../include/Controller/MediaPlaylistManagerController.h"
MediaPlaylistManagerController::MediaPlaylistManagerController(PlaylistManager& playlistModel, MediaFileManager& mediafileManager, FolderManager& folderManager, std::shared_ptr<BaseView> mediaManagerView, std::shared_ptr<BaseView> playlistManagerView, std::shared_ptr<BaseView> playlistHandlerView) 
    : playlistManager(playlistModel), mediaFileManager(mediafileManager), folderManager(folderManager), mediaManagerView(mediaManagerView), playlistManagerView(playlistManagerView), playlistHandlerView(playlistHandlerView){}

void MediaPlaylistManagerController::addMediaPlaylistController(std::string playlistName, std::shared_ptr<MediaPlaylistController> mediaPlaylistController) 
{
    if(listMediaPlaylistController.find(playlistName) == listMediaPlaylistController.end()){
        listMediaPlaylistController[playlistName] = mediaPlaylistController;
    }
    std::cerr << "Error: MediaPlaylistController already exists.\n";
}

std::shared_ptr<MediaPlaylistController> MediaPlaylistManagerController::getMediaPlaylistController(const std::string& playlistName) 
{
    auto currentMediaPlaylistController = listMediaPlaylistController.find(playlistName);
    if (currentMediaPlaylistController == listMediaPlaylistController.end()) {
        std::cerr << "Error: MediaPlaylistController not found.\n";
        return nullptr;
    }
    return currentMediaPlaylistController->second;
}

std::string MediaPlaylistManagerController::createPlaylist(std::string namePlaylist) 
{
    try {

        if (namePlaylist.empty()) {
            throw InvalidPlaylistNameException("Playlist name cannot be empty.");
        }

        if (std::all_of(namePlaylist.begin(), namePlaylist.end(), [](unsigned char c) { return std::isdigit(c); })) {
            throw InvalidPlaylistNameException("Playlist name cannot contain only numbers. Provided: " + namePlaylist);
        }

        if (!std::all_of(namePlaylist.begin(), namePlaylist.end(), [](unsigned char c) { 
                return std::isalnum(c) || std::isspace(c); 
            })) {
            throw InvalidPlaylistNameException("Playlist name must contain only alphabetic, numeric characters, or spaces. Provided: " + namePlaylist);
        }

        if (playlistManager.checkPlaylistName(namePlaylist)) {
            throw DuplicatePlaylistException(namePlaylist);
        }

        playlistManager.addPlaylist(std::make_shared<Playlist>(namePlaylist));
        return "OK";
    } catch (const PlaylistManagerException& e) {
        return e.what();
    }
}

bool MediaPlaylistManagerController::deletePlaylist(std::string playlistName) 
{
    bool check = false;

    if (!playlistManager.checkPlaylistName(playlistName)) {
        return check;
    }

    if(playlistManagerView->showConfirmMenu("Do you want to delete this file: " + playlistName + " ?")){
        playlistManager.deletePlaylist(playlistName);
        check = true;
    }
    return check;
    
}

std::string MediaPlaylistManagerController::displayAllPlaylist(){return playlistManagerView->displayAllPlaylist(playlistManager);}

void MediaPlaylistManagerController::handlerPlaylistManager() {
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

            choice = playlistManagerView->showMenuWithPlaylist(playlistManager.getAllPlaylist());

            switch (choice) {
            case CREATE_PLAYLIST:
            {   
                    std::string name;
                    std::cout << "Enter the name of the playlist (type '0' to cancel): ";
                    std::getline(std::cin, name); 
                    if (name == "0") {
                        break;
                    }
                    message = createPlaylist(name);
                    if(message == "OK"){
                        message = "Playlist" + name +" created successfully.";
                    }else{
                        error = message;
                        message = "";
                        continue;
                    }
                
                break;
            }
            case DELETE_PLAYLIST:
            {
                while(true){
                    std::string playlistName;
                    playlistName = displayAllPlaylist();
                    if (playlistName == "") {
                        break;
                    }
                    if(deletePlaylist(playlistName)){
                        message = "Playlist deleted" + playlistName + " successfully.";
                    }else{
                        continue;
                    }
                    break;
                }
                break;
            }
            case DISPLAY_ALL_PLAYLIST:
            {
                std::string playlistName;
                playlistName = displayAllPlaylist();
                if (playlistName == "") {
                    break;
                }
                if(listMediaPlaylistController.find(playlistName) == listMediaPlaylistController.end()){
                    std::shared_ptr<MediaPlaylistController> mediaPlaylistController = std::make_shared<MediaPlaylistController>(mediaFileManager, folderManager, playlistManager.getPlaylist(playlistName), mediaManagerView, playlistHandlerView);
                    listMediaPlaylistController[playlistName] = mediaPlaylistController;
                }
                listMediaPlaylistController[playlistName]->handlerPlaylist();
                break;
            }
            case EXIT_MENU_PLAYLIST_MANAGER:
                return;
            default:
                throw InvalidChoiceException();
            }
        } catch (const PlaylistManagerException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }catch (const InvalidChoiceException &e) {
            std::cerr << "Error: " << e.what() << '\n'; 
        }
    }
}
