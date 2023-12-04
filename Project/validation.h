#include "objects.h"

#ifndef PROJECT_VALIDATION_H
#define PROJECT_VALIDATION_H

class Validations {
public:
    static bool validPlaneId(int id);
    static bool validRunwayId(int, int&);
    static bool validPlaneMinRunwayDistance(int, int, int);
    static bool validNumericData(const string&, string&);
    static FlightStatus validStrFlightStatus();
    static bool credentialsInformation(string&, string&, bool);
};

#endif //PROJECT_VALIDATION_H
