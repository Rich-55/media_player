#ifndef MEDIA_FILE_HANDLER_VIEW_H
#define MEDIA_FILE_HANDLER_VIEW_H

#include "BaseView.h"
class MediaFileHandlerView : public BaseView {
    public:
        MediaFileHandlerView();

        int showMenu() override;

        void displayDetailMediaFile(std::shared_ptr<MediaFile>, std::string) override;
        std::pair<std::string, std::string> displayMenuAddMetadata(std::shared_ptr<MediaFile>, std::string) override;
        std::pair<std::string, std::string> displayMenuEditMetadata(std::shared_ptr<MediaFile>, std::string) override;
        std::string displayMenuDeleteMetadata(std::shared_ptr<MediaFile> ) override;


};

#endif
