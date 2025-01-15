#ifndef MEDIA_FILE_HANDLER_VIEW_H
#define MEDIA_FILE_HANDLER_VIEW_H

#include "ViewBase.h"

#define Add_metadata 1
#define Edit_metadata 2
#define Delete_metadata 3
#define Back_to_Main_menu 0

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
