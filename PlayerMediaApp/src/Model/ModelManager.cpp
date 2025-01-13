#include "../../include/Model/ModelManager.h"

ModelManager::ModelManager(){}

// void ModelManager::addMetaData(std::vector<std::string> listPathName){

// }

MetadataManager &ModelManager::getMetadataManager()
{
    return this->metadataManager;
}

PlaylistManager &ModelManager::getPlaylistManager()
{
    return this->playlistManager;
}

std::shared_ptr<Playlist> ModelManager::getPlaylist(std::string name)
{
    return playlistManager.getPlaylist(name);
}

std::shared_ptr<MediaFile> ModelManager::getMediaFile(std::string fileName)
{
    return metadataManager.getMediaFile(fileName);
}