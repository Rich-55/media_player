#ifndef METADATA_VIEW_H
#define METADATA_VIEW_H
#include "../Model/MetadataManager.h"
#include "../Model/MediaFile.h"
#include "../View/ViewBase.h"

class MetadataView : public ViewBase{
    private:

    public:
        MetadataView();

        void showMetadata(std::shared_ptr<MediaFile>);

        void showAllMetadata(MetadataManager);
};

#endif
