#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../View/ScanView.h"
#include "../Model/MetadataManager.h"
#include "../View/ViewBase.h"
#include <memory>
class MediaScannerController{
    private:
        MetadataManager& metadataManager;
        std::shared_ptr<ViewBase> scanView;
        std::vector<std::string> listPaths;
        
    public:
        MediaScannerController(MetadataManager&, std::shared_ptr<ViewBase>);

        void scanUSBDevices(std::vector<std::string>&);
        void scanHomeDirectory(std::vector<std::string>&);

        void scan();

};

#endif
