#include "../../include/View/ViewBase.h"


ViewBase::ViewBase(){}
        
void ViewBase::showMenu(){}

void ViewBase::scanHomeDirectory(std::vector<std::string>&){}

void ViewBase::scanUSBDevices(std::vector<std::string>&){}

void ViewBase::displayAllMediaFile(MetadataManager){}

void ViewBase::displayAllMediaFileOfAudio(MetadataManager) {}

void ViewBase::displayAllMediaFileOfVideo(MetadataManager) {}

ViewBase::~ViewBase(){}
