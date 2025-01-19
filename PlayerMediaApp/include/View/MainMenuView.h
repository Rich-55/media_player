#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#include "BaseView.h"
#include <functional>

class MainMenuView : public BaseView {
private:

public:
    MainMenuView();

    int showMenu() override;

    int showMenuWithPlayer(MediaFileManager, std::string) override;
};

#endif 
