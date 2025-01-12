#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
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

        virtual ~ViewBase();

};

#endif