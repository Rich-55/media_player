#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "../Model/MetadataManager.h"
#include "../Model/MediaFile.h"
#include "../Model/PlaylistManager.h"
class ModelManager{
    private:
        MetadataManager metadataManager;
        PlaylistManager playlistManager;
    public:
        ModelManager();
    
        MetadataManager &getMetadataManager();

        PlaylistManager &getPlaylistManager();

        std::shared_ptr<Playlist> getPlaylist(std::string);

        std::shared_ptr<MediaFile> getMediaFile(std::string);


};


#endif
