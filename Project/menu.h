#include "flight.h"
using namespace std;

#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

extern int cnt;
class Menu {
private:
    static void login();
    static void registration();
    static void adminMenu();
    static void userMenu();
    static void commonPrompt();
public:
    static void welcome();
};

#endif //PROJECT_MENU_H