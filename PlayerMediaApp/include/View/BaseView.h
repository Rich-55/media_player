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
#include "../Controller/PlayerController.h"
#include "../utils/ConfigLoader.h"
class BaseView{
    private:
    public:
        BaseView();
        
        virtual int showMenu() = 0;
        
        virtual void showNotificationMessage(std::string, std::string);
        virtual bool showConfirmMenu(std::string);

        virtual int showMenuWithPlayer(MediaFileManager, std::shared_ptr<PlayerController>&, std::string, std::string&);
        //view for media file manager
        virtual std::string displayAllMediaFile(MediaFileManager);
        virtual std::string displayAllMediaFileOfAudio(MediaFileManager);
        virtual std::string displayAllMediaFileOfVideo(MediaFileManager);
        virtual int showMenuWithMediaList(MediaFileManager);
        //view for metadata file
        virtual void displayDetailMediaFile(std::shared_ptr<MediaFile>, std::string );
        virtual std::pair<std::string, std::string> displayMenuAddMetadata(std::shared_ptr<MediaFile>, std::string);
        virtual std::pair<std::string, std::string> displayMenuEditMetadata(std::shared_ptr<MediaFile>, std::string);
        virtual std::string displayMenuDeleteMetadata(std::shared_ptr<MediaFile> );
        //view for playlist handler
        virtual std::string displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>);
        virtual int showMenuWithMediaListInPlaylist(std::shared_ptr<Playlist>);
        virtual std::pair<std::string, std::string> displayAllFolder(std::pair<std::unordered_set<std::string> , std::unordered_set<std::string> >);
        //view for playlist manager
        virtual std::string displayAllPlaylist(PlaylistManager);
        virtual int showMenuWithPlaylist(std::vector<std::shared_ptr<Playlist>>);
        virtual std::string showMenuCreatePlaylist();
        //view for scan manager
        virtual int showListFolder(std::vector<std::string>);
        virtual int showListUSBName(std::vector<std::string>);
        virtual void setListPathNameIsAdded(std::unordered_set<std::string>);
        virtual void showFileAdded();

        virtual ~BaseView();

};

#endif
