#ifndef METADATA_VIEW_H
#define METADATA_VIEW_H
#include "../Model/MetadataManager.h"
#include "../Model/MediaFile.h"
#include "../View/BaseCurrentPlayView.h"

class MetadataView : public BaseCurrentPlayView{
    private:

    public:
        MetadataView();

        void showMetadata(std::shared_ptr<MediaFile>);

        void showMenu() override;

};

#endif