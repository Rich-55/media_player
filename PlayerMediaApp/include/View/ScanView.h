#ifndef SCAN_VIEW_H
#define SCAN_VIEW_H

#include "../View/ViewBase.h"
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>


class ScanView : public ViewBase{
    private:

    public:
        ScanView();

        void scanHomeDirectory(std::vector<std::string>&) override; 

        void scanUSBDevices(std::vector<std::string>&) override;

        // std::vector<std::string> scanData();

        void showMenu() override;
};
#endif
