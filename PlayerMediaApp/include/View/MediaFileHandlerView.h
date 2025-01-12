#ifndef MEDIA_FILE_HANDLER_VIEW_H
#define MEDIA_FILE_HANDLER_VIEW_H

#include "ViewBase.h"

class MediaFileHandlerView : public ViewBase {
    public:
        MediaFileHandlerView();

        void showMenu() override;

        void displayDetailMediaFile(std::shared_ptr<MediaFile> ) override;
        void displayMenuAddMetadata(std::shared_ptr<MediaFile> ) override;
        void displayMenuEditMetadata(std::shared_ptr<MediaFile> ) override;
        void displayMenuDeleteMetadata(std::shared_ptr<MediaFile> ) override;


};

#endif
