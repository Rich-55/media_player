#ifndef MEDIA_FILE_HANDLER_VIEW_H
#define MEDIA_FILE_HANDLER_VIEW_H

#include "../View/ViewBase.h"
#include "../Controller/MediaFileController.h"

class MediaFileHandlerView : public ViewBase {
    private:

    public:
        MediaFileHandlerView();
        void showMenu() override;
};

#endif
