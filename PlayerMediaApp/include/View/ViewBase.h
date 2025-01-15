#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include "../Model/MetadataManager.h"
#include "../Model/PlaylistManager.h"
class ViewBase{
    private:
    public:
        ViewBase();
        
        virtual void showMenu() = 0;

        virtual void displayAllMediaFile(MetadataManager);
        virtual void displayAllMediaFileOfAudio(MetadataManager);
        virtual void displayAllMediaFileOfVideo(MetadataManager);

        virtual void displayDetailMediaFile(std::shared_ptr<MediaFile> );
        virtual void displayMenuAddMetadata(std::shared_ptr<MediaFile> );
        virtual void displayMenuEditMetadata(std::shared_ptr<MediaFile> );
        virtual void displayMenuDeleteMetadata(std::shared_ptr<MediaFile> );

        virtual void displayAllPlaylist(PlaylistManager);
        virtual void displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>);

        virtual void setListPathNameIsAdded(std::unordered_set<std::string>);
        virtual void showFileAdded();

        virtual ~ViewBase();

};

#endif
