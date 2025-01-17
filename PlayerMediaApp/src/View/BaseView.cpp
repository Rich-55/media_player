#include "../../include/View/BaseView.h"

BaseView::BaseView(){}
        
std::string BaseView::displayAllMediaFile(MediaFileManager){return "";}

void BaseView::displayAllMediaFileOfAudio(MediaFileManager){}

void BaseView::displayAllMediaFileOfVideo(MediaFileManager){}


void BaseView::displayDetailMediaFile(std::shared_ptr<MediaFile>, std::string){}
std::pair<std::string, std::string> BaseView::displayMenuAddMetadata(std::shared_ptr<MediaFile>, std::string){return {};}
std::pair<std::string, std::string> BaseView::displayMenuEditMetadata(std::shared_ptr<MediaFile>, std::string){return {};}
std::string BaseView::displayMenuDeleteMetadata(std::shared_ptr<MediaFile>){return "";}

void BaseView::displayAllPlaylist(PlaylistManager){}
void BaseView::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>){}

int BaseView::showListFolder(std::vector<std::string>){return -1;}
int BaseView::showListUSBName(std::vector<std::string>) {return -1;}
void BaseView::setListPathNameIsAdded(std::unordered_set<std::string>){}
void BaseView::showFileAdded(){}

BaseView::~BaseView(){}
