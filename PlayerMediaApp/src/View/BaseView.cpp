#include "../../include/View/BaseView.h"

BaseView::BaseView(){}
        
void BaseView::displayAllMediaFile(MediaFileManager){}

void BaseView::displayAllMediaFileOfAudio(MediaFileManager){}

void BaseView::displayAllMediaFileOfVideo(MediaFileManager){}


void BaseView::displayDetailMediaFile(std::shared_ptr<MediaFile>){}
void BaseView::displayMenuAddMetadata(std::shared_ptr<MediaFile>){}
void BaseView::displayMenuEditMetadata(std::shared_ptr<MediaFile>){}
void BaseView::displayMenuDeleteMetadata(std::shared_ptr<MediaFile>){}

void BaseView::displayAllPlaylist(PlaylistManager){}
void BaseView::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>){}

void BaseView::setListPathNameIsAdded(std::unordered_set<std::string>){}
void BaseView::showFileAdded(){}

BaseView::~BaseView(){}
