#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
class ViewBase{
    private:
    public:
        ViewBase();
        
        virtual void showMenu() = 0;

        virtual void scanHomeDirectory(std::vector<std::string>&); 

        virtual void scanUSBDevices(std::vector<std::string>&);

        virtual void displayAllMediaFile(MetadataManager);
 
        virtual void displayAllMediaFileOfAudio(MetadataManager);
 
        virtual void displayAllMediaFileOfVideo(MetadataManager);


        virtual ~ViewBase();

};

#endif
