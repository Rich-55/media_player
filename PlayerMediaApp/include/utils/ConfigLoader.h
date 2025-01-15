#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

//MainMenu
#define METADATA_FILE_HANDLER 1
#define MEDIA_FILE_MANAGER 2 
#define PLAYLIST_HANDLER 3
#define PLAYLIST_MANAGER 4
#define EXIT_MAIN_MENU 0

//Scan Menu
#define SCAN_HOME_DIRECTORY 1
#define SCAN_USB 2
#define EXIT_MENU_SCAN 0

//MediaFile Menu
#define ADD_METADATA 1
#define EDIT_METADATA 2
#define DELETE_METADATA 3
#define EXIT_MENU_METADATA_EDIT 0

//MediaFileManager Menu
#define ADD_FILE_PATH 1
#define ADD_FOLDER_PATH 2
#define DELETE_FILE 3
#define SHOW_ALL_MEDIAFILE 4
#define SHOW_ALL_MEDIAFILE_AUDIO 5
#define SHOW_ALL_MEDIAFILE_VIDEO 6
#define EXIT_MENU_MEDIAFILE_MANAGER 0

//Playlist Menu
#define ADD_MEDIA_FILE_TO_PLAYLIST 1
#define DELETE_MEDIA_FILE_FROM_PLAYLIST 2
#define DISPLAY_ALL_MEDIA_FILE_FROM_PLAYLIST 3
#define EXIT_MENU_PLAYLIST_HANDLER 0

//PlaylistManager Menu
#define CREATE_PLAYLIST 1
#define DELETE_PLAYLIST 2
#define DISPLAY_ALL_PLAYLIST 3
#define EXIT_MENU_PLAYLIST_MANAGER 0

#include <iostream>

class InvalidChoiceException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid input. Please enter a valid choice show on screen.";
    }
};

#endif
