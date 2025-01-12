#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../View/ScanView.h"
#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include <memory>
#include <unordered_set>
class MediaScannerController{
    private:
        MetadataManager& metadataManager;
        std::shared_ptr<ViewBase> scanView;
        std::unordered_set<std::string> listPaths;
        
    public:
        MediaScannerController(MetadataManager&, std::shared_ptr<ViewBase>);

        std::unordered_set<std::string> getListPaths();

        void scanUSBDevices();
        void scanHomeDirectory();



};

#endif
