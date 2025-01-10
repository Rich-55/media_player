#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../View/ScanView.h"
#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include <memory>
class MediaScannerController{
    private:
        std::shared_ptr<ViewBase> scan;
        std::vector<std::string> listPathNames;
        MetadataManager manager;
        
    public:
        MediaScannerController(std::shared_ptr<ViewBase>, MetadataManager);

        void scanView();

};

#endif
