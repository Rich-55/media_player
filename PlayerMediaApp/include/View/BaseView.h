#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include "../Model/MediaFileManager.h"
#include "../Model/PlaylistManager.h"
#include "../utils/ConfigLoader.h"
class BaseView{
    private:
    public:
        BaseView();
        
        virtual int showMenu() = 0;

        virtual std::string displayAllMediaFile(MediaFileManager);
        virtual void displayAllMediaFileOfAudio(MediaFileManager);
        virtual void displayAllMediaFileOfVideo(MediaFileManager);

        virtual void displayDetailMediaFile(std::shared_ptr<MediaFile> );
        virtual void displayMenuAddMetadata(std::shared_ptr<MediaFile> );
        virtual void displayMenuEditMetadata(std::shared_ptr<MediaFile> );
        virtual void displayMenuDeleteMetadata(std::shared_ptr<MediaFile> );

        virtual void displayAllPlaylist(PlaylistManager);
        virtual void displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>);

        virtual void setListPathNameIsAdded(std::unordered_set<std::string>);
        virtual void showFileAdded();

        virtual ~BaseView();

};

#endif
