#include "../../include/Model/Playlist.h"

Playlist::Playlist(std::string name) : name(name) {}

std::string Playlist::getName() const
{
    return this->name;
}

bool Playlist::checkMediaFile(std::string file)
{
    for(const auto &mediaFile : listMediaFiles)
    {
        if(mediaFile->getName() == file)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<MediaFile>> Playlist::getListMediaFiles()
{
    return this->listMediaFiles;
}

void Playlist::addMediaFile(std::shared_ptr<MediaFile> file) 
{
    for(const auto &mediaFile : listMediaFiles)
    {
        if(mediaFile->getPath() == file->getPath())
        {
            std::cerr << "File already exists in the playlist.\n";
            return;
        }
    }
    listMediaFiles.push_back(file);
    std::cout << "Media file added to playlist.\n";
}

void Playlist::deleteMediaFile(std::string file)
{
    for(auto it = listMediaFiles.begin(); it != listMediaFiles.end(); ++it)
    {
        if((*it)->getName() == file)
        {
            listMediaFiles.erase(it);
            std::cout << "Media file deleted from playlist.\n";
            return;
        }
    }
    std::cerr << "File not found in the playlist.\n";
}

Playlist::~Playlist() {}