#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#include "BaseView.h"

class MainMenuView : public BaseView {
private:

public:
    MainMenuView();

    int showMenu() override;

    int showMenuWithPlayer(MediaFileManager, std::shared_ptr<PlayerController>&, std::string, std::string&) override;
};

#endif 
