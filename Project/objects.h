#include "files.h"

#ifndef PROJECT_OBJECTS_H
#define PROJECT_OBJECTS_H

extern regex numericRegex;
extern regex dateRegex;
extern regex timeRegex;

class ObjectsManaging {
public:
    static bool createPlane();
    static bool createFlight();
    static bool createRunway();
    static bool deleteObject(const string&, const string&, const string&, int&);
    static bool deletePlane(const string&, const string&, const string&);
    static void searchByDestination(const string&,const string&);
    static void searchForAPlane();
    static void searchForARunway();
};

#endif //PROJECT_OBJECTS_H
