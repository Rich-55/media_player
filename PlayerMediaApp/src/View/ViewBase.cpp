#include "../../include/View/ViewBase.h"

ViewBase::ViewBase(){}
        
void ViewBase::displayAllMediaFile(MetadataManager){}

void ViewBase::displayAllMediaFileOfAudio(MetadataManager){}

void ViewBase::displayAllMediaFileOfVideo(MetadataManager){}


void ViewBase::displayDetailMediaFile(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuAddMetadata(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuEditMetadata(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuDeleteMetadata(std::shared_ptr<MediaFile>){}

ViewBase::~ViewBase(){}
