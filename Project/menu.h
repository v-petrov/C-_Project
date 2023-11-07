#include "flight.h"
using namespace std;

class Menu {
public:
    static void welcome();
    static void login();
    static void registration();
    static bool isUsernameAvailable(const string&, const string&, bool);
    static void saveUser(const string&, const string&);
    static void adminMenu();
    static void userMenu();
    static bool createPlane();
    static void addPlaneToFile(const Plane&);
    static bool createFlight();
    static bool validPlaneId(int id);
    static bool validNumericData(const string&, string&);
    static bool credentialsInformation(string& username, string& password, bool isLogin);
    static void addFlightToFile(const Flight&);
    static void searchByDestination(const string&,const string&);
    static void searchByAirline(const string&);
    static void printFlights(const vector<Flight*>& flights);
    static int* loadIds();
    static void saveIds();
};

#ifndef PROJECT_COMMUNICATION_H
#define PROJECT_COMMUNICATION_H

#endif //PROJECT_COMMUNICATION_H
