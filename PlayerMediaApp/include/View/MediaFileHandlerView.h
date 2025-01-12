#ifndef MEDIA_FILE_HANDLER_VIEW_H
#define MEDIA_FILE_HANDLER_VIEW_H

#include "ViewBase.h"

class MediaFileHandlerView : public ViewBase {
    public:
        MediaFileHandlerView();

        void showMenu() override;

        void displayDetailMediaFile(std::shared_ptr<MediaFile> ) override;

        void displayMenuEditMediaFile(std::shared_ptr<MediaFile> ) override;

        void displayMenuAddNewKey(std::shared_ptr<MediaFile> ) override;

        void displayMenuDeleteKey(std::shared_ptr<MediaFile> ) override;


};

#endif
