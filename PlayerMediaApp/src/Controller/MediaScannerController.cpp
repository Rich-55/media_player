#include "../../include/Controller/MediaScannerController.h"

MediaScannerController::MediaScannerController(std::shared_ptr<ViewBase> view, MetadataManager manager): scan(view), manager(manager){}

void MediaScannerController::scanView()
{
    int choice = 0;
    std::cin >> choice;

    if (choice == 1) {
        scan->scanHomeDirectory(this->listPathNames);
    } else if (choice == 2) {
        scan->scanUSBDevices(this->listPathNames);
    } else {
        std::cerr << "Invalid menu choice.\n";
    }
    
    for(size_t i = 0; i < listPathNames.size(); ++i){
        std::string path = listPathNames[i];
         size_t lastSlashPos = path.find_last_of("/");

        std::string fileName = (lastSlashPos != std::string::npos) ? path.substr(lastSlashPos + 1) : path;

        size_t lastDotPos = path.find_last_of(".");
        std::string extension = (lastDotPos != std::string::npos) ? path.substr(lastDotPos + 1) : "";

        if(extension == "mp4"){
            manager.addMediaFile(listPathNames[i], "Video");  
        }else{
            manager.addMediaFile(listPathNames[i], "Audio");  
        }

        std::cout << "File name " << i + 1 << " is added " << fileName << std::endl;   
           
    }
   
}