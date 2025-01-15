#ifndef PLAYLIST_HANDLER_EXCEPTION_H
#define PLAYLIST_HANDLER_EXCEPTION_H

#include <stdexcept>
#include <regex>
#include <string>
#include <iostream>

// Exception cho tên file không hợp lệ (Chỉ alphanumeric và các định dạng file hợp lệ)
void validateMediaFileName(const std::string& fileName) {
    if (fileName.empty() || !std::regex_match(fileName, std::regex("^[A-Za-z0-9_]+\\.(mp3|mp4|avi|mkv)$"))) {
        throw std::invalid_argument("Invalid file name. Please use alphanumeric characters and supported file extensions (.mp3, .mp4, .avi, .mkv).");
    }
}

// Exception cho khi người dùng nhập tên file không tồn tại khi xóa
void ensureMediaFileExists(std::shared_ptr<Playlist> playlist, const std::string& fileName) {
    bool fileFound = false;
    for (const auto& mediaFile : playlist->getListMediaFiles()) {
        if (mediaFile->getName() == fileName) {
            fileFound = true;
            break;
        }
    }
    if (!fileFound) {
        throw std::runtime_error("File not found in the playlist. Unable to delete.");
    }
}

// Exception cho playlist trống khi người dùng cố gắng hiển thị media files
void ensurePlaylistNotEmpty(std::shared_ptr<Playlist> playlist) {
    if (playlist->getListMediaFiles().empty()) {
        throw std::runtime_error("Playlist is empty. Unable to display media files.");
    }
}

// Exception cho khi người dùng nhập tên file không hợp lệ khi thêm file vào playlist
void ensureFileDoesNotExistInPlaylist(std::shared_ptr<Playlist> playlist, const std::shared_ptr<MediaFile> mediaFile) {
    for (const auto& existingFile : playlist->getListMediaFiles()) {
        if (existingFile->getName() == mediaFile->getName()) {
            throw std::runtime_error("File already exists in the playlist. Cannot add.");
        }
    }
}

// Exception khi người dùng nhập sai input (chỉ 'Y' hoặc 'N')
void validateInputYorN(const std::string& input) {
    if (input.length() != 1 || (input != "Y" && input != "N")) {
        throw std::invalid_argument("Invalid input. Please enter 'Y' or 'N' only.");
    }
}

#endif