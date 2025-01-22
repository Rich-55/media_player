#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#include "BaseView.h"

class MainMenuView : public BaseView {
private:

public:
    MainMenuView();

    int showMenu() override;

    int showMenuWithPlayer(MediaFileManager mediaFileManager, std::shared_ptr<PlayerController>& playerController, 
    std::string typePlay, std::string& error) override;
};

#endif 
