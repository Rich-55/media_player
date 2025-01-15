#include "../../include/Controller/MediaPlaylistManagerController.h"
#include "../../include/utils/PlayListManagerException.h" // Include the exception handling header

MediaPlaylistManagerController::MediaPlaylistManagerController(PlaylistManager& m, std::shared_ptr<ViewBase> v) 
    : playlistManager(m), playlistManagerView(v) {}

// Cập nhật validateYorNInput tại điểm gọi nhập liệu
void MediaPlaylistManagerController::createPlaylist() {
    std::string choice;
    bool validInput = false;

    // Lặp lại cho đến khi người dùng nhập 'Y' hoặc 'N'
    while (!validInput) {
        std::cout << "Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ";
        std::cin >> choice;

        try { 
            validateYorNInput(choice); // Validate input for 'Y' or 'N' only
            validInput = true; // Đặt flag thành true nếu input hợp lệ
            if (choice == "Y" || choice == "y") {
                std::string name;
                std::cout << "Enter the name of the playlist: ";
                std::cin >> name;

                validatePlaylistName(name); // Validate the playlist name
                if (playlistManager.playlistExists(name)) {
                    throw std::runtime_error("Playlist already exists with the same name.");
                }
                playlistManager.addPlaylist(std::make_shared<Playlist>(name));
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void MediaPlaylistManagerController::deletePlaylist() {
    std::string name;
    std::cout << "Enter the name of the playlist to delete: ";
    std::cin >> name;

    try {
        validatePlaylistName(name); // Validate the playlist name
        ensurePlaylistExists(playlistManager, name); // Check if the playlist exists
        playlistManager.deletePlaylist(name);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MediaPlaylistManagerController::displayAllPlaylist() {
    try {
        ensurePlaylistNotEmpty(playlistManager); // Ensure playlists are not empty
        playlistManagerView->displayAllPlaylist(playlistManager);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

