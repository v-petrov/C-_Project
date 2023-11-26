#include "menu.h"

#ifndef PROJECT_FILES_H
#define PROJECT_FILES_H

class FilesHandling {
private:
    static int* loadIds();
public:
    static bool isUsernameAvailable(const string&, const string&, bool);
    static void saveUser(const string&, const string&);
    static void addPlaneToFile(const Plane&);
    static void addRunwayToFile(const Runway&);
    static void addFlightToFile(const Flight&);
    static void saveIds(const int*);
    static int* setIds();
};

#endif //PROJECT_FILES_H
