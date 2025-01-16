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

        void setListPathNameIsAdded(std::unordered_set<std::string>);
        void showFileAdded() override;

        
};
#endif
