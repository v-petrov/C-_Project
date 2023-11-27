#include "menu.h"

#ifndef PROJECT_FILES_H
#define PROJECT_FILES_H

class FilesHandling {
private:
    static int* loadIds();
public:
    static void addDataToFile(const WriteToFile&, const string&);
    static bool isUsernameAvailable(const string&, const string&, bool);
    static void saveUser(const string&, const string&);
    static void saveIds(const int*);
    static int* setIds();
};

#endif //PROJECT_FILES_H
