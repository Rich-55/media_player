#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include "BaseView.h"
#include "MainMenuView.h"
#include "ScanView.h"
#include "MediaFileHandlerView.h"
#include "MediaFileManagerView.h"
#include "PlaylistHandlerView.h"
#include "PlaylistManagerView.h"
#include "UartView.h"


class ViewManager{
    private:
        std::unordered_map<std::string, std::shared_ptr<BaseView>> views; 
        std::shared_ptr<BaseView> currentView;
    public: 

        ViewManager();

        std::shared_ptr<BaseView> getView(std::string name);

        ~ViewManager();
    
};

#endif
