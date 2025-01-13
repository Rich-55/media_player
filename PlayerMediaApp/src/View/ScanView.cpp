#include "../../include/View/ScanView.h"

ScanView::ScanView() {}

void ScanView::showMenu(){
    showFileAdded();
    std::cout << "Menu Scan View\n";
    std::cout << "1. Scan Home Directory\n";
    std::cout << "2. Scan USB Devices\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}


void ScanView::setListPathNameIsAdded(std::unordered_set<std::string> listPathName)
{
    this->listPathNameIsAdded = listPathName;
}

void ScanView::showFileAdded()
{
    std::cout << "List of files add new:\n";

    if (this->listPathNameIsAdded.empty()) {
        std::cout << "No files added.\n";
        return;
    }
    for(const auto &path : this->listPathNameIsAdded){
        std::cout << path << std::endl;
    }

}



