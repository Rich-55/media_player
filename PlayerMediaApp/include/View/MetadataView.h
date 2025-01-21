#ifndef METADATA_VIEW_H
#define METADATA_VIEW_H
#include "../Model/MediaFileManager.h"
#include "../Model/MediaFile.h"
#include "../View/BaseCurrentPlayView.h"

class MetadataView : public BaseCurrentPlayView{
    private:

    public:
        MetadataView();

        void showMenu() override;

        void showMetadata(std::shared_ptr<MediaFile>);

        

};

#endif
