#include "menu.h"
#include "validation.h"
#include "files.h"
#include "objects.h"
using namespace std;

int cnt = 0;
void Menu::welcome() {
    cout << "Welcome to our application!\nWe are here to help you choose the most suitable plane for your needs.\n"
            "You can choose from many different airlines.Go and check that for yourself.\n"<< endl;
    char c;
    cnt = -1;
    do {
        cnt++;
        if (cnt >= 3) {
            cout << "Your session ended. Too many tries!\n" << endl;
            return;
        }
        cout << "Login or Registration[L/R]:  "<< endl;
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        c = static_cast<char>(toupper(c));
        switch (c) {
            case 'L':
                login();
                break;
            case 'R':
                registration();
                break;
            default:
                cout << "You need to provide a correct answer. Try again!" << endl;
        }
    } while (c != 'L' && c != 'R');
}
void Menu::login() {
    bool res = false;
    char c = 'Y';
    cnt = 0;
    while (!res && c == 'Y') {
        cnt++;
        string username, password;
        res = Validations::credentialsInformation(username, password, true);

        if (res) {
            cout << "Login successful" << endl;
            if (username == "admin") {
                adminMenu();
            } else {
                userMenu();
            }
        } else {
            cout << "No such username or password have been found!\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return;
            }
            cout << "Do you want to try one more time[Y/N]:" << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        }
    }
}
void Menu::registration() {
    bool res = false;
    char c = 'Y';
    cnt = 0;
    while (!res && c == 'Y') {
        cnt++;
        string username, password;
        res = Validations::credentialsInformation(username, password, false);
        if (res) {
            FilesHandling::saveUser(username, password);
            cout << "Registration has been successful!" << endl;
            userMenu();
        } else {
            cout << "Username already exists\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return;
            }
            cout << "Do you want to try one more time[Y/N]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        }
    }
}
void Menu::adminMenu() {
    cout << "You have logged in as an admin\n" << endl;
    cout << "What do you want to do?\n" << endl;
    cout << "Do you want to add a plan to the database[P]: " << endl;
    cout << "Do you want to add a flight to the database[F]: " << endl;
    cout << "Do you want to add a runway to the database[R]: " << endl;
    cout << "Do you want to update a flight's status[U]: " << endl;
    cout << "Do you want to delete a flight, a plane or a runway from the database[D]: " << endl;
    cout << "If you want to exit the admin menu press anything else[...]" << endl;
    char c;
    int id;
    bool res;
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    c = static_cast<char>(toupper(c));
    do {
        if (c == 'P') {
            res = ObjectsManaging::createPlane();
            if (!res) {
                return;
            }
            cout << "The plane has been added. Do you want to add another plane, flight or a runway[P/F/R]: " << endl;
            cout << "If you want to exit the admin menu press anything else[...]" << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'F') {
            res = ObjectsManaging::createFlight();
            if (!res) {
                return;
            }
            cout << "The flight has been added. Do you want to add another flight, plane or a runway[F/P/R]: " << endl;
            cout << "If you want to exit the admin menu press anything else[...]" << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'R') {
            res = ObjectsManaging::createRunway();
            if (!res) {
                return;
            }
            cout << "The runway has been added. Do you want to add another runway, plane or a flight[R/P/F]: " << endl;
            cout << "If you want to exit the admin menu press anything else[...]" << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'D') {
            cout << "Do you want to delete a flight, a plane or a runway[P/F/R]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
            if (c == 'P') {
                res = ObjectsManaging::deletePlane("Enter plane's id that you want to delete: ", "plane.json", "Plane");
            } else if (c == 'F') {
                res = ObjectsManaging::deleteObject("Enter flight's id that you want to delete: ", "flight.json", "Flight", id);
            } else if (c == 'R') {
                res = ObjectsManaging::deleteObject("Enter runway's id that you want to delete: ", "runway.json", "Runway", id);
            } else {
                cout << "Please provide a correct input!" << endl;
                return;
            }
            if (!res) {
                return;
            }
            cout << "The data has been deleted. Do you want to delete something else[D]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'U') {
            string flightId;
            if (!Validations::validNumericData("Enter flight's id: ", flightId)) {
                return;
            }
            if (ObjectsManaging::changeFlightStatus(stoi(flightId))) {
                cout << "Flight's status has been updated successfully. Do you want to update another one[U]: " << endl;
                cin >> c;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                c = static_cast<char>(toupper(c));
            } else {
                return;
            }
        }
    } while (c == 'P' || c == 'F' || c == 'D' || c == 'R' || c == 'U');
}
void Menu::userMenu() {
    char c;
    do {
        cout << "What do you want to do?\n"
                "Search for a flight for a specific destination[D].\n"
                "Search for a plane[P].\n"
                "Search for a runway[R].\n"
                "If you want to exit the user menu press anything else[...]"<< endl;
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        c = static_cast<char>(toupper(c));
        switch (c) {
            case 'D': {
                cout << "Enter starting destination: " << endl;
                string sDestination;
                getline(cin, sDestination);
                cout << "Enter ending destination: " << endl;
                string eDestination;
                getline(cin, eDestination);
                ObjectsManaging::searchByDestination(sDestination, eDestination);
                break;
            } case 'P': {
                ObjectsManaging::searchForAPlane();
                break;
            } case 'R': {
                ObjectsManaging::searchForARunway();
            } default: {
                break;
            }
        }
        cout << "Do you want to continue searching[Y/N]: " << endl;
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        c = static_cast<char>(toupper(c));
    } while(c == 'Y');
}