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
    } else {
        std::cout << "File Name: " << media->getName() << std::endl;
        std::cout << "File Type: " << media->getType() << std::endl;
        std::cout << "File Size: " << media->getSize() << std::endl;
        std::cout << "Track Name: " << media->getTrackname() << std::endl;
        std::cout << "Artist: " << media->getArtist() << std::endl;
        std::cout << "Album: " << media->getAlbum() << std::endl;
        std::cout << "Genre: " << media->getGenre() << std::endl;
        std::cout << "Duration: " << media->getDuration() << std::endl;
        std::cout << "Bitrate: " << media->getBitrate() << std::endl;
        std::cout << "Sample Rate: " << media->getSampleRate() << std::endl;
    }
}

void MetadataView::showMenu(){
    std::cout << "Menu Metadata View" << std::endl;
}

