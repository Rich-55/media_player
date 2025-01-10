#ifndef MEDIA_SCANNER_CONTROLLER_H
#define MEDIA_SCANNER_CONTROLLER_H

#include "../View/ScanView.h"
#include "../View/ViewBase.h"
#include <memory>
class MediaScannerController{
    private:
        std::shared_ptr<ViewBase> scan;
    public:
        MediaScannerController(std::shared_ptr<ViewBase>);

        std::vector<std::string> scanView();

};

#endif
