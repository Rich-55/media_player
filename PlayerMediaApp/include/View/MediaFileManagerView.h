#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include <iostream>
#include <vector>
#include <iomanip>
#include "../View/ViewBase.h"
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../Model/MetadataManager.h"

#define Add_File_By_File_Path 1
#define Add_File_By_Folder_Path 2
#define Delete_File 3
#define View_All_Media_File 4
#define View_All_Audio_Media_File 5
#define View_All_Video_Media_File 6
#define Back_to_menu 0


class MediaFileManagerView : public ViewBase{
private:

public:

    MediaFileManagerView();
    
    void displayAllMediaFile(MetadataManager) override;
    void displayAllMediaFileOfAudio(MetadataManager) override;
    void displayAllMediaFileOfVideo(MetadataManager) override;

    void showMenu() override;


};

#endif

