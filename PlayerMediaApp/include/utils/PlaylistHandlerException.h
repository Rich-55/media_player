#ifndef PLAYLIST_HANDLER_EXCEPTION_H
#define PLAYLIST_HANDLER_EXCEPTION_H

#include <exception>
#include <string>
#include <limits>

// Base exception class
class PlaylistHandlerException : public std::exception {
public:
    explicit PlaylistHandlerException(const std::string& msg) : message(msg) {}

    virtual ~PlaylistHandlerException() noexcept = default;

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};

// Exception for file not found in playlist
class FileNotFoundInPlaylistException : public PlaylistHandlerException {
public:
    explicit FileNotFoundInPlaylistException(const std::string& fileName)
        : PlaylistHandlerException("File not found in playlist: " + fileName) {}
};

// Exception for duplicate media file
class DuplicateMediaFileException : public PlaylistHandlerException {
public:
    explicit DuplicateMediaFileException(const std::string& fileName)
        : PlaylistHandlerException("Duplicate media file: " + fileName) {}
};

// Exception for invalid file name
class InvalidFileNameException : public PlaylistHandlerException {
public:
    explicit InvalidFileNameException(const std::string& fileName)
        : PlaylistHandlerException("Invalid file name: " + fileName) {}
};

#endif
