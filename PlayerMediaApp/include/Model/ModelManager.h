#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "MediaFile.h"
#include "MediaFileManager.h"
#include "PlaylistManager.h"
#include "FolderManager.h"

class ModelManager{
    private:
        MediaFileManager mediaFileManager;
        PlaylistManager playlistManager;
        FolderManager folderManager;
    public:
        ModelManager();
    
        MediaFileManager &getMediaFileManager();

        PlaylistManager &getPlaylistManager();

        FolderManager &getFolderManager();

        std::shared_ptr<Playlist> getPlaylist(std::string playlistName);

        std::shared_ptr<MediaFile> getMediaFile(std::string fileName);

};


#endif
