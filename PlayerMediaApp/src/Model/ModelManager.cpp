#include "../../include/Model/ModelManager.h"

ModelManager::ModelManager(){}

MediaFileManager &ModelManager::getMediaFileManager(){ return this->mediaFileManager; }

PlaylistManager &ModelManager::getPlaylistManager(){ return this->playlistManager; }

FolderManager &ModelManager::getFolderManager(){ return this->folderManager;}

std::shared_ptr<Playlist> ModelManager::getPlaylist(std::string playlistName){ return playlistManager.getPlaylist(playlistName);}

std::shared_ptr<MediaFile> ModelManager::getMediaFile(std::string fileName){ return mediaFileManager.getMediaFile(fileName);}