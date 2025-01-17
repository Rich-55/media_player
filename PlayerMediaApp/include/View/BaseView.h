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

        virtual void showSuccessMessage(std::string);
        virtual bool showConfirmMenu(std::string);

        virtual std::string displayAllMediaFile(MediaFileManager);
        virtual std::string displayAllMediaFileOfAudio(MediaFileManager);
        virtual std::string displayAllMediaFileOfVideo(MediaFileManager);

        virtual void displayDetailMediaFile(std::shared_ptr<MediaFile>, std::string );
        virtual std::pair<std::string, std::string> displayMenuAddMetadata(std::shared_ptr<MediaFile>, std::string);
        virtual std::pair<std::string, std::string> displayMenuEditMetadata(std::shared_ptr<MediaFile>, std::string);
        virtual std::string displayMenuDeleteMetadata(std::shared_ptr<MediaFile> );

        virtual void displayAllPlaylist(PlaylistManager);
        virtual void displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>);

        virtual int showListFolder(std::vector<std::string>);
        virtual int showListUSBName(std::vector<std::string>);
        virtual void setListPathNameIsAdded(std::unordered_set<std::string>);
        virtual void showFileAdded();

        virtual ~BaseView();

};

#endif
