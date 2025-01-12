#include "../../include/View/ViewBase.h"

ViewBase::ViewBase(){}
        
void ViewBase::displayAllMediaFile(MetadataManager){}

void ViewBase::displayAllMediaFileOfAudio(MetadataManager){}

void ViewBase::displayAllMediaFileOfVideo(MetadataManager){}


void ViewBase::displayDetailMediaFile(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuEditMediaFile(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuAddNewKey(std::shared_ptr<MediaFile>){}
void ViewBase::displayMenuDeleteKey(std::shared_ptr<MediaFile>){}

ViewBase::~ViewBase(){}
