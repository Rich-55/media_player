#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m ,CLIManager v) : model(m), view(v){}

void ControllerManager::ScanData()
{   
    std::shared_ptr<ViewBase> scan = std::make_shared<ScanView>();

    view.switchView(scan);

    MediaScannerController scanController(scan, model.getMetadataManager());

    scanController.scanView();
}

void ControllerManager::viewAllDataAdded()
{
    std::shared_ptr<ViewBase> viewMediaFileManager = std::make_shared<MediaFileManagerView>();

    view.switchView(viewMediaFileManager);


    MediaFileController fileController(model.getMetadataManager(), viewMediaFileManager);

    fileController.showMediaFile();

}
