#include "../../include/View/MetadataView.h"

MetadataView::MetadataView(){}

void MetadataView::showMetadata(std::shared_ptr<MediaFile> media)
{   
    if(media->getType() == "Video"){
        std::cout << "File Name: " << media->getName() << std::endl;
        std::cout << "File Type: " << media->getType() << std::endl;
        std::cout << "File Size: " << media->getSize() << std::endl;
        std::cout << "File Duration: " << media->getDuration() << std::endl;
        std::cout << "File Codec: " << media->getCodec() << std::endl;
        std::cout << "File Bitrate: " << media->getBitrate() << std::endl;
        std::cout << "File Resolution: " << media->getResolution() << std::endl;
    }

}

void MetadataView::showMenu(){
    std::cout << "Menu Metadata View" << std::endl;
}

