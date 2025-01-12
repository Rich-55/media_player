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
        void showMenu() override;
};
#endif