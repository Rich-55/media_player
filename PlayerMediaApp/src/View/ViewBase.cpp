#include "../../include/View/ViewBase.h"

ViewBase::ViewBase(){}
        
void ViewBase::displayAllMediaFile(MetadataManager){}

void ViewBase::displayAllMediaFileOfAudio(MetadataManager){}

void ViewBase::displayAllMediaFileOfVideo(MetadataManager){}


void ViewBase::displayDetailMediaFile(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuAddMetadata(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuEditMetadata(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuDeleteMetadata(std::shared_ptr<MediaFile>){}

void ViewBase::displayAllPlaylist(PlaylistManager){}
void ViewBase::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>){}

void ViewBase::setListPathNameIsAdded(std::unordered_set<std::string>){}
void ViewBase::showFileAdded(){}

ViewBase::~ViewBase(){}
