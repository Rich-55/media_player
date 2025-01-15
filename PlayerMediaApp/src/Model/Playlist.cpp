#include "../../include/Model/Playlist.h"

Playlist::Playlist(std::string name) : name(name) {}

std::string Playlist::getName() const
{
    return this->name;
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

bool Playlist::deleteMediaFile(std::string& fileName)
{
    for (auto it = listMediaFiles.begin(); it != listMediaFiles.end(); ++it)
    {
        if ((*it)->getName() == fileName)
        {
            listMediaFiles.erase(it);
            return true;
        }
    }
    return false;
}

bool Playlist::isEmpty() const {
    return listMediaFiles.empty(); // Kiểm tra xem vector có rỗng không
}
Playlist::~Playlist() {}