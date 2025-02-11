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
        virtual ~ModelManager();

        virtual MediaFileManager &getMediaFileManager();

        virtual PlaylistManager &getPlaylistManager();

        virtual FolderManager &getFolderManager();

        virtual std::shared_ptr<Playlist> getPlaylist(std::string playlistName);

        virtual std::shared_ptr<MediaFile> getMediaFile(std::string fileName);

        virtual std::shared_ptr<MediaFile> getMediaFileByPath(std::string pathName);

};


#endif
