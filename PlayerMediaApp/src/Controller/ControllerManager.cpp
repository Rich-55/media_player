#include "../../include/Controller/ControllerManager.h"

ControllerManager::ControllerManager(ModelManager m ,CLIManager v) : model(m), view(v){}

void ControllerManager::ScanData() {
    std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

    if (!scanView) {
        std::cerr << "Error: ScanView not found!\n";
        return;
    }
    view.switchView("ScanView");

    MediaScannerController scan(model.getMetadataManager(), scanView);

    scan.scan();
}

void ControllerManager::viewAllDataAdded()
{
    std::shared_ptr<ViewBase> viewMediaFileManager = std::make_shared<MediaFileManagerView>();

    view.switchView(viewMediaFileManager);


    MediaFileController fileController(model.getMetadataManager(), viewMediaFileManager);

    fileController.showMediaFile();

}
