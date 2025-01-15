#include "../../include/View/MainMenuView.h"
#include <iostream>

MainMenuView::MainMenuView() {}


void MainMenuView::showMenu() {
 
    std::cout << "============Main Menu==========" << std::endl;
    std::cout << "1. MetadataFile Handler" << std::endl;
    std::cout << "1. MediaFile Manager" << std::endl;
    std::cout << "3. Playlist Handler" << std::endl;
    std::cout << "4. Playlist Manager" << std::endl;
    
        
    std::cout << "0. Exit" << std::endl;
    
}
