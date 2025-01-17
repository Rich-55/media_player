#ifndef SCAN_VIEW_H
#define SCAN_VIEW_H

#include "../View/BaseView.h"
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>


class ScanView : public BaseView{
    private:
        std::unordered_set<std::string> listPathNameIsAdded;
    public:
        ScanView();

        int showMenu() override;
        
        int showListFolder(std::vector<std::string>) override;
        int showListUSBName(std::vector<std::string>) override;
        void setListPathNameIsAdded(std::unordered_set<std::string>) override;
        void showFileAdded() override;

        
};
#endif
