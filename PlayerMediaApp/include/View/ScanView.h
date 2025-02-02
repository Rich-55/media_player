#ifndef SCAN_VIEW_H
#define SCAN_VIEW_H

#include "BaseView.h"

class ScanView : public BaseView{
    private:
        std::unordered_set<std::string> listPathNameIsAdded;
    public:
        ScanView();

        int showMenu() override;
        
        int showListFolder(std::vector<std::string> listFolder) override;
        int showListUSBName(std::vector<std::string> usbDevices) override;
        void setListPathNameIsAdded(std::unordered_set<std::string> listPathName) override;
        void showFileAdded() override;
};
#endif
