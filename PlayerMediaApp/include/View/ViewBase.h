#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include "../Model/MetadataManager.h"

class ViewBase{
    private:
    public:
        ViewBase();
        
        virtual void showMenu() = 0;

        virtual void displayAllMediaFile(MetadataManager);
        virtual void displayAllMediaFileOfAudio(MetadataManager);
        virtual void displayAllMediaFileOfVideo(MetadataManager);

        virtual void displayDetailMediaFile(std::shared_ptr<MediaFile> );
        virtual void displayMenuEditMediaFile(std::shared_ptr<MediaFile> );
        virtual void displayMenuAddNewKey(std::shared_ptr<MediaFile> );
        virtual void displayMenuDeleteKey(std::shared_ptr<MediaFile> );

        virtual ~ViewBase();

};

#endif
