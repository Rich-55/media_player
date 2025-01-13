#include "../../include/View/MediaFileHandlerView.h"
#include <limits>

MediaFileHandlerView::MediaFileHandlerView(){
    // std::cout <<"added" << std::endl;
}

void MediaFileHandlerView::showMenu() {
    std::cout << "========== Media File Handler ==========" << std::endl;
    std::cout << "1. Add Media File" << std::endl;
    std::cout << "2. Edit Media File" << std::endl;
    std::cout << "3. Remove Media File" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

