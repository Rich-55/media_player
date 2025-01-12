#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#include "ViewBase.h"
#include <functional>

class MainMenuView : public ViewBase {
private:

public:
    MainMenuView();

    void showMenu() override;
};

#endif // MAIN_MENU_VIEW_H