#include "menu.h"
#include "files.h"
int main() {
    int* ids = FilesHandling::setIds();
    Menu::welcome();
    FilesHandling::saveIds(ids);
    return 0;
}