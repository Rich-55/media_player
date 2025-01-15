#ifndef PLAYLIST_MANAGER_EXCEPTION_H
#define PLAYLIST_MANAGER_EXCEPTION_H

#include <stdexcept>
#include <regex>
#include <string>
 
// Exception for invalid input ('Y' or 'N' only)
void validateYorNInput(const std::string& input) {
    if (input.length() != 1 || (input != "Y" && input != "N")) {
        throw std::invalid_argument("Invalid input. Please enter 'Y' or 'N' only.");
    }
}

// Exception for invalid playlist name
void validatePlaylistName(const std::string& name) {
    if (name.empty() || !std::regex_match(name, std::regex("^[A-Za-z0-9_ ]+$"))) {
        throw std::invalid_argument("Invalid playlist name. Please use only letters, numbers, spaces, and underscores.");
    }
}

// Exception for checking if playlist exists
void ensurePlaylistExists(PlaylistManager& playlistManager, const std::string& name) {
    if (!playlistManager.playlistExists(name)) {
        throw std::runtime_error("Playlist does not exist. Unable to proceed.");
    }
}

// Exception for empty playlist manager
void ensurePlaylistNotEmpty(PlaylistManager& playlistManager) {
    if (!playlistManager.checkPlaylist()) {
        throw std::runtime_error("Playlist is empty. Unable to display playlists.");
    }
}

#endif // PLAYLIST_MANAGER_EXCEPTION_H
