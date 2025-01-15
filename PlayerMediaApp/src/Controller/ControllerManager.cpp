    #include "../../include/Controller/ControllerManager.h"

    ControllerManager::ControllerManager(ModelManager m, CLIManager v) 
        : model(m), view(v), scannerController(nullptr), mediaFileHandlerController(nullptr), mediaFileManagerController(nullptr), mediaPlaylistController(nullptr), mediaPlaylistManagerController(nullptr) {}

    void ControllerManager::ScanData() 
    {   
        if(model.getFolderManager().getListFolderDirectory().empty() && model.getFolderManager().getListFolderUSB().empty()){
            std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

            if (!scanView) {
                std::cerr << "Error: ScanView not found or failed to initialize.\n";
                return;
            }
            if (!scannerController) {
                scannerController = std::make_unique<MediaScannerController>(model.getMetadataManager(), model.getFolderManager(), scanView);
            }

            std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
            
            if (!mediaFileManagerView) {
                std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
                return;
            }
            if(!mediaFileManagerController){
                mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
            }

            while(true){
                if(!scannerController->getListPaths().empty()){
                    scanView->setListPathNameIsAdded(mediaFileManagerController->getListFileAdded());
                    mediaFileManagerController->clearListFileAdded();
                }
                view.switchView("ScanView");
                int choice; std::cin >> choice;
                switch (choice)
                {
                case Scan_Home_Directory:
                    scannerController->scanHomeDirectory();
                    mediaFileManagerController->addData(scannerController->getListPaths());
                    break;
                case Scan_USB_Devices:
                    scannerController->scanUSBDevices();
                    mediaFileManagerController->addData(scannerController->getListPaths());
                    break;
                case Back_to_Main_Menu:
                    return;
                default:
                    break;
                }
            }
        }else{
            std::shared_ptr<ViewBase> scanView = view.getView("ScanView");

            if (!scanView) {
                std::cerr << "Error: ScanView not found or failed to initialize.\n";
                return;
            }
            if (!scannerController) {
                scannerController = std::make_unique<MediaScannerController>(model.getMetadataManager(), model.getFolderManager(), scanView);
            }

            std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
            
            if (!mediaFileManagerView) {
                std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
                return;
            }
            if(!mediaFileManagerController){
                mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
            }


            if(!scannerController->checkFolderDirectory()){
                mediaFileManagerController->addData(scannerController->getlistFolderDirectory());
            }
            if(!scannerController->checkFolderUSB()){
                mediaFileManagerController->addData(scannerController->getlistFolderUSB());
            }


        }

    }

    void ControllerManager::mediaFileManager() 
    {
        std::shared_ptr<ViewBase> mediaFileManagerView = view.getView("MediaFileManagerView");
        
        if (!mediaFileManagerView) {
            std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
            return;
        }
        if(!mediaFileManagerController){
            mediaFileManagerController = std::make_unique<MediaFileManagerController>(model.getMetadataManager(), mediaFileManagerView);
        }
        int choice; 
        while(true){
            view.switchView("MediaFileManagerView");
            std::cin >> choice;
            switch (choice)
            {
            case Add_File_By_File_Path:
                {
                std::cout << "Enter your path of file to add: ";
                std::string pathFile;
                std::cin >> pathFile;
                mediaFileManagerController->addDataFile(pathFile);
                break;
                }
            case Add_File_By_Folder_Path:
                {
                std::cout << "Enter your folder path to add: ";
                std::string pathFolder;
                std::cin >> pathFolder;
                mediaFileManagerController->addData(scannerController->scanFolder(pathFolder));
                break;
                }
            case Delete_File:
                {
                std::cout << "Enter the name of the file you want to delete: ";
                std::string fileName;
                std::cin >> fileName;
                mediaFileManagerController->deleteData(fileName);
                break;
                }
            case View_All_Media_File:
                mediaFileManagerController->showAllMediaFile();
                break;
            case View_All_Audio_Media_File:
                mediaFileManagerController->showAllMediaFileOfAudio();
                break;
            case View_All_Video_Media_File:
                mediaFileManagerController->showAllMediaFileOfVideo();
                break;    
            case Back_to_menu:
                return;
            default:
                break;
            }
        }
    }

    void ControllerManager::metadataFileHandler()
    {
        std::shared_ptr<ViewBase> mediaFileHandlerView = view.getView("MediaFileHandlerView");

        if (!mediaFileHandlerView) {
            std::cerr << "Error: MediaFileManagerView not found or failed to initialize.\n";
            return;
        }

        if(mediaFileManagerController){
            mediaFileManagerController->showAllMediaFile();
        }

        std::cout << "Enter the name of the file you want to edit: ";
        std::string fileName;
        std::cin >> fileName;

        if(!mediaFileHandlerController){
            if(model.getMediaFile(fileName) == nullptr){
                std::cerr << "File not found!";
                return;
            }
            mediaFileHandlerController = std::make_unique<MediaFileController>(model.getMediaFile(fileName), mediaFileHandlerView);
        }
        int choice;
        while(true){
            mediaFileHandlerController->getDetailMediaFile();
            view.switchView("MediaFileHandlerView");
            std::cin >> choice;
            switch (choice)
            {
            case Add_metadata:
                mediaFileHandlerController->addMetadata();
                break;
            case Edit_metadata:
                mediaFileHandlerController->editMetadata();
                break;
            case Delete_metadata:
                mediaFileHandlerController->deleteMetadata();
                break;    
            case Back_to_Main_menu:
                return;
            default:
                break;
            }
        }

    }

    void ControllerManager::playlistManager()
    {
        std::shared_ptr<ViewBase> playlistManagerView = view.getView("PlaylistManagerView");

        if (!playlistManagerView) {
            std::cerr << "Error: PlaylistManagerView not found or failed to initialize.\n";
            return;
        }

        if (!mediaPlaylistManagerController) {
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(model.getPlaylistManager(), playlistManagerView);
        }

        if (!model.getPlaylistManager().checkPlaylist()) {
            std::cout << "No playlists found. Do you want to create a new playlist? (Y/N): ";
            char choice;
            std::cin >> choice;

            if (choice == 'N' || choice == 'n') {
                return; 
            }

            mediaPlaylistManagerController->createPlaylist(); 
        }

        int choice;
        while (true) {
            view.switchView("PlaylistManagerView");
            std::cin >> choice;

            switch (choice) {
            case Create_new_playlist:
                mediaPlaylistManagerController->createPlaylist(); 
                break;
            case Delete_playlist_from_mediafile:
            {
                mediaPlaylistManagerController->deletePlaylist();
                break;
            }
            case Show_all_playlist:
                mediaPlaylistManagerController->displayAllPlaylist(); 
                break;
            case Exit:
                return; 
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    void ControllerManager::playlistHandler() {
        std::shared_ptr<ViewBase> playlistManagerView = view.getView("PlaylistManagerView");

        if (!playlistManagerView) {
            std::cerr << "Error: PlaylistHandlerView not found or failed to initialize.\n";
            return;
        }

        if (!mediaPlaylistManagerController) {
            mediaPlaylistManagerController = std::make_unique<MediaPlaylistManagerController>(model.getPlaylistManager(), playlistManagerView);
        }
 
        if (!model.getPlaylistManager().checkPlaylist()) {
            std::cout << "Playlist is empty. Are you sure you want to create a new playlist? (Y/N): ";
            
            std::string choice;
            std::cin.ignore();
            getline(std::cin, choice);

            if (choice == "N" || choice == "n") {
                return; 
            }

            mediaPlaylistManagerController->createPlaylist(); 
        }

        mediaPlaylistManagerController->displayAllPlaylist();

        std::shared_ptr<ViewBase> playlistHandlerView = view.getView("PlaylistHandlerView");

        if (!playlistHandlerView) {
            std::cerr << "Error: PlaylistHandlerView not found or failed to initialize.\n";
            return;
        }

        std::cout << "Enter the name of the playlist you want to edit: ";
        std::string playlistName;
        std::cin >> playlistName;

        if (!model.getPlaylistManager().checkPlaylistName(playlistName)) {
            std::cerr << "Playlist not found!\n";
            return;
        }

        if (!mediaPlaylistController) {
            mediaPlaylistController = std::make_unique<MediaPlaylistController>(model.getPlaylist(playlistName), playlistHandlerView);
        }

        int choice;
        while (true) {
            view.switchView("PlaylistHandlerView");
            std::cin >> choice;
            switch (choice) {
            case Add_MediaFile:
            { 
                std::cout << "Enter the name of the file you want to add: ";
                std::string fileName;
                std::cin >> fileName;
                std::shared_ptr<MediaFile> mediaFile = model.getMediaFile(fileName);
                if(mediaFile == nullptr){
                    std::cerr << "File not found!\n";
                    return;
                }
                mediaPlaylistController->addMediaFile(mediaFile);
            }
                
                break;
            case Delete_MediaFile:
                mediaPlaylistController->deleteMediaFile();
                break;
            case Display_All_MediaFiles:
                mediaPlaylistController->displayAllMediaFiles();
                break;
            case exit:
                return; 
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
