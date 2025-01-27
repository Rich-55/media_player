#ifndef MEDIA_FILE_HANDLER_VIEW_H
#define MEDIA_FILE_HANDLER_VIEW_H

#include "BaseView.h"

class MediaFileHandlerView : public BaseView {
    public:
        MediaFileHandlerView();

        int showMenu() override;

        void showNotificationMessage(std::string message, std::string type) override;
        bool showConfirmMenu(std::string message) override;

        void displayDetailMediaFile(std::shared_ptr<MediaFile> mediaFile, std::string message) override;
        std::pair<std::string, std::string> displayMenuAddMetadata(std::shared_ptr<MediaFile> mediaFile, std::string exception) override;
        std::pair<std::string, std::string> displayMenuEditMetadata(std::shared_ptr<MediaFile> mediaFile, std::string exception) override;
        std::string displayMenuDeleteMetadata(std::shared_ptr<MediaFile> mediaFile) override;
};

#endif
