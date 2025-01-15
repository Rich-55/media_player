#ifndef PLAYLIST_MANAGER_EXCEPTION_H
#define PLAYLIST_MANAGER_EXCEPTION_H

#include <exception>
#include <string>
#include <limits>

class PlaylistManagerException : public std::exception {
public:
    explicit PlaylistManagerException(const std::string& msg) : message(msg) {}

    virtual ~PlaylistManagerException() noexcept = default;

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};

class DuplicatePlaylistException : public PlaylistManagerException {
public:
    explicit DuplicatePlaylistException(const std::string& name)
        : PlaylistManagerException("Playlist already exists: " + name) {}
};

class PlaylistNotFoundException : public PlaylistManagerException {
public:
    explicit PlaylistNotFoundException(const std::string& name)
        : PlaylistManagerException("Playlist not found: " + name) {}
};

class InvalidPlaylistNameException : public PlaylistManagerException {
public:
    explicit InvalidPlaylistNameException(const std::string& name)
        : PlaylistManagerException("Invalid playlist name: " + name) {}
};

#endif
