#include "menu.h"
using namespace std;

regex numericRegex("^[0-9]+(\\.[0-9]+)?$");
regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
regex timeRegex("\\d{2}:\\d{2}");

int cnt;

void Menu::saveIds() {
    ofstream file("ids.txt");
    if (file.is_open()) {
        file << Plane::getIdCnt() << "," << Flight::getIdCnt() << "," << Runway::getIdCnt() << endl;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}

int* Menu::loadIds() {
    int* idPointer = new int[3];
    ifstream file("ids.txt");
    if (file.is_open()) {
        string line;
        getline(file, line);
        size_t pos = line.find(',');
        if (pos != string::npos) {
            string planeIds = line.substr(0, pos);
            string flightIds = line.substr(pos + 1);
            string runwayIds = line.substr(pos + 3);
            idPointer[0] = stoi(planeIds);
            idPointer[1] = stoi(flightIds);
            idPointer[2] = stoi(runwayIds);
        }
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    return idPointer;
}

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
        res = credentialsInformation(username, password, true);

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
        res = credentialsInformation(username, password, false);
        if (res) {
            saveUser(username, password);
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

bool Menu::credentialsInformation(string& username, string& password, bool isLogin) {
    cout << "Enter your username: " << endl;
    cin >> username;
    cout << "Enter your password: " << endl;
    cin >> password;
    return isUsernameAvailable(username, password, isLogin);
}
bool Menu::isUsernameAvailable(const string& username, const string& password, bool isLogin) {
    ifstream file("admin.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string currUsername = line.substr(0, pos);
                if (isLogin) {
                    string currPassword = line.substr(pos + 1);
                    if (currUsername == username && currPassword == password) {
                        file.close();
                        return true;
                    }
                } else {
                    if (currUsername == username) {
                        file.close();
                        return false;
                    }
                }
            }
        }
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    return !isLogin;
}
void Menu::saveUser(const string &username, const string &password) {
    ofstream file("admin.txt", ios::app);
    if (file.is_open()) {
        file << username << "," << password << endl;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}
void Menu::adminMenu() {
    cout << "You have logged in as an admin\n" << endl;
    cout << "What do you want to do?\n" << endl;
    cout << "Do you want to add a plan to the database[P]: " << endl;
    cout << "Do you want to add a flight to the database[F]: " << endl;
    cout << "Do you want to add a runway to the database[R]: " << endl;
    cout << "Do you want to delete a flight or a plane from the database[D]: " << endl;
    cout << "If you want to exit the admin menu press anything else[...]" << endl;
    char c;
    int id;
    bool res;
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    c = static_cast<char>(toupper(c));
    do {
        if (c == 'P') {
            res = createPlane();
            if (!res) {
                return;
            }
            cout << "The plane has been added. Do you want to add another plane[P]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'F') {
            res = createFlight();
            if (!res) {
                return;
            }
            cout << "The flight has been added. Do you want to add another flight[F]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'R') {
            res = createRunway();
            if (!res) {
                return;
            }
            cout << "The runway has been added. Do you want to add another runway[R]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
        } else if (c == 'D') {
            cout << "Do you want to delete a flight, a plane or a runway[P/F/R]: " << endl;
            cin >> c;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            c = static_cast<char>(toupper(c));
            if (c == 'P') {
                res = deletePlane("Enter plane's id that you want to delete: ", "plane.json", "Plane");
            } else if (c == 'F') {
                res = deleteObject("Enter flight's id that you want to delete: ", "flight.json", "Flight", id);
            } else if (c == 'R') {
                res = deleteObject("Enter runway's id that you want to delete: ", "runway.json", "Runway", id);
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
        }
    } while (c == 'P' || c == 'F' || c == 'D' || c == 'R');
}

bool Menu::createPlane() {
    string manufacturer, model, minRunwayDistance;
    cnt = 0;

    cout << "Enter plain's manufacturer: " << endl;
    cin >> manufacturer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter plain's model: " << endl;
    cin >> model;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!validNumericData("Enter plain's minimum runway distance: ", minRunwayDistance)) {
        return false;
    }
    PlaneClass planeClass(manufacturer, model, stod(minRunwayDistance));

    string airline, oneKmCost, tankVolume, allCost, averageSpeed;

    cout << "Enter plain's airline: " << endl;
    cin >> airline;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!validNumericData("Enter plain's one kilometer cost: ", oneKmCost)) {
        return false;
    }
    if (!validNumericData("Enter plain's total cost: ", allCost)) {
        return false;
    }
    if (!validNumericData("Enter plain's tank volume: ", tankVolume)) {
        return false;
    }
    if (!validNumericData("Enter plain's average speed: ", averageSpeed)) {
        return false;
    }
    char c = 't';

    while (true) {
        cnt++;
        cout << "What type of plane do you want to add[PASSENGERS, BUSINESS, CARGO]: " << endl;
        string planeType;
        cin >> planeType;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        PlaneType pt = stringToEnum(planeType);

        switch (pt) {
            case PASSENGERS: {
                string firstClass, passengerSeats;
                if (!validNumericData("Enter plain's passenger seats: ", passengerSeats)) {
                    return false;
                }
                cout << "Enter if the plain has first class[TRUE/FALSE]: " << endl;
                cin >> firstClass;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                bool firstClassBool = (firstClass == "TRUE" || firstClass == "true");

                PassengerPlane passengerPlane(planeClass, airline, stod(oneKmCost), stod(allCost), stoi(tankVolume), stod(averageSpeed), stoi(passengerSeats), firstClassBool);
                const Plane& plane = passengerPlane;
                addPlaneToFile(plane);
                break;
            } case BUSINESS: {
                string passengerSeats, flightEntertainment, privateSuites;
                if (!validNumericData("Enter plain's passenger seats: ", passengerSeats)) {
                    return false;
                }
                cout << "Enter if the plain has flight entertainment[TRUE/FALSE]: " << endl;
                cin >> flightEntertainment;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                bool flightEntertainmentBool = (flightEntertainment == "TRUE" || flightEntertainment == "true");

                cout << "Enter if plain has private suites[TRUE/FALSE]: " << endl;
                cin >> privateSuites;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                bool privateSuitesBool = (privateSuites == "TRUE" || privateSuites == "true");

                BusinessPlane businessPlane(planeClass, airline, stod(oneKmCost), stod(allCost), stoi(tankVolume), stod(averageSpeed), stoi(passengerSeats), flightEntertainmentBool, privateSuitesBool);
                const Plane& plane = businessPlane;
                addPlaneToFile(plane);
                break;
            } case CARGO: {
                string capacity, numberOfCompartments, temperatureControl;
                if (!validNumericData("Enter plain's capacity: ", capacity)) {
                    return false;
                }
                if (!validNumericData("Enter plain's number of compartments: ", numberOfCompartments)) {
                    return false;
                }
                cout << "Enter if plain has temperature control[TRUE/FALSE]: " << endl;
                cin >> temperatureControl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                bool temperatureControlBool = (temperatureControl == "TRUE" || temperatureControl == "true");

                CargoPlane cargoPlane(planeClass, airline, stod(oneKmCost), stod(allCost), stoi(tankVolume), stod(averageSpeed), stod(capacity), stoi(numberOfCompartments), temperatureControlBool);
                const Plane& plane = cargoPlane;
                addPlaneToFile(plane);
                break;
            } default:
                cout << "No such plane type. Please provide a correct one!\n" << endl;
                if (cnt == 3) {
                    cout << "Your session ended. Too many tries!\n" << endl;
                    return false;
                }
                c = 'f';
                break;
        }
        if (c == 't') {
            break;
        }
    }
    return true;
}
bool Menu::deletePlane(const string& text, const string& fileName, const string& text1) {
    int id = 0;
    deleteObject(text, fileName, text1, id);

    ifstream inputFile("flight.json");
    nlohmann::json existingData;

    if (inputFile.is_open()) {
        inputFile >> existingData;
        inputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    auto predicate = [id](const auto& flight) {
        return flight["plainId"] == id;
    };
    existingData.erase(remove_if(existingData.begin(), existingData.end(), predicate), existingData.end());

    ofstream outputFile("flight.json");
    if (outputFile.is_open()) {
        outputFile << existingData.dump(4) << endl;
        outputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    return true;
}
void Menu::addPlaneToFile(const Plane& plane) {
    nlohmann::json jsonPlane = plane.toJson();

    if (!filesystem::exists("plane.json")) {
        ofstream createFile("plane.json");
        if (createFile.is_open()) {
            createFile << "[]" << std::endl;
            createFile.close();
        } else {
            throw ios_base::failure("File couldn't be created");
        }
    }

    ifstream inputFile("plane.json");
    nlohmann::json existingData;

    if (inputFile.is_open()) {
        inputFile >> existingData;
        inputFile.close();
    }
    else {
        throw ios_base::failure("File couldn't be open");
    }

    existingData.push_back(jsonPlane);

    ofstream outputFile("plane.json");

    if (outputFile.is_open()) {
        outputFile << existingData.dump(4) << endl;
        outputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}

bool Menu::createRunway() {
    string airportName, distance;
    cout << "Enter runways airport name: " << endl;
    cin >> airportName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!validNumericData("Enter runways distance: ", distance)) {
        return false;
    }
    Runway runway(airportName, stoi(distance));
    addRunwayToFile(runway);
    return true;
}
bool Menu::createFlight() {
    string flightStatus, startingDestination, endingDestination, totalDistance;
    FlightStatus flightStatusEnum;
    bool res;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter flights status[FINISHED, ONGOING, INCOMING]: " << endl;
        cin >> flightStatus;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        flightStatusEnum = stringToEnumFlight(flightStatus);
        if (flightStatusEnum == ERROR) {
            cout << "Please provide a correct status.\n";
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while (flightStatusEnum == ERROR);

    cout << "Enter flights starting destination: " << endl;
    cin >> startingDestination;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int takeOffRunwayId;
    int takeOffRunwayDistance;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter take off runway id for the flight: " << endl;
        cin >> takeOffRunwayId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        res = validRunwayId(takeOffRunwayId, takeOffRunwayDistance);
        if (!res) {
            cout << "Please provide a correct runway's id for the flight.\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while(!res);
    cout << "Enter flights ending destination: " << endl;
    cin >> endingDestination;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int landingRunwayId;
    int landingRunwayDistance;
    cnt = 0;
        do {
            cnt++;
            cout << "Enter landing runway id for the flight: " << endl;
            cin >> landingRunwayId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (takeOffRunwayId == landingRunwayId) {
                cout << "You can't have the same runway id for both landing and taking off.\n";
                if (cnt == 3) {
                    cout << "Your session ended. Too many tries!\n" << endl;
                    return false;
                }
                res = false;
                continue;
            }
            res = validRunwayId(landingRunwayId, landingRunwayDistance);
            if (!res) {
                cout << "Please provide a correct runway's id for the flight.\n" << endl;
                if (cnt == 3) {
                    cout << "Your session ended. Too many tries!\n" << endl;
                    return false;
                }
            }
        } while(!res);
    if (!validNumericData("Enter flights total distance(km): ", totalDistance)) {
        return false;
    }

    string dateInput, timeInput;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter date of the flight(YYYY-MM-DD): " << endl;
        cin >> dateInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter time of the flight(HH:MM): " << endl;
        cin >> timeInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        res = regex_match(dateInput, dateRegex) && regex_match(timeInput, timeRegex);
        if (!res) {
            cout << "Please provide a correct format of the data!.\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while (!res);

    int planeId;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter plane's id for the flight: " << endl;
        cin >> planeId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        res = validPlaneId(planeId);
        if (!res) {
            cout << "Please provide a correct plane's id for the flight.\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }    
        } else {
            if (!validPlaneMinRunwayDistance(planeId, takeOffRunwayDistance, landingRunwayDistance)) {
                cout << "Plane's minimal runway distance is longer than one or both of the runways! Please choose different plane." << endl;
                res = false;
            }
        }
    } while(!res);
    Flight flight(flightStatusEnum, startingDestination, takeOffRunwayId, endingDestination, landingRunwayId, stod(totalDistance), dateInput, timeInput, planeId);
    addFlightToFile(flight);
    return true;
}
bool Menu::validPlaneMinRunwayDistance(int planeId, int takeOffDistance, int landingDistance) {
    ifstream file("plane.json");
    nlohmann::json planes;
    if (file.is_open()) {
        file >> planes;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    for (auto plane : planes) {
        if (plane["id"] == planeId) {
            nlohmann::json planeClass = plane["planeClass"];
            double minDistance = planeClass["minRunwayDistance"];
            return minDistance <= takeOffDistance && minDistance <= landingDistance;
        }
    }
    return false;
}
bool Menu::deleteObject(const string& text, const string& fileName, const string& text1, int& id) {
    string flightId;
    if (!validNumericData(text, flightId)) {
        return false;
    }
    id = stoi(flightId);

    ifstream file(fileName);
    nlohmann::json flightData;
    if (file.is_open()) {
        file >> flightData;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    auto it = find_if(flightData.begin(), flightData.end(), [&flightId](const auto& flight) {
        return flight["id"] == stoi(flightId);
    });

    if (it != flightData.end()) {
        char p;
        cout << "CONFIRMATION!!!DO YOU WANT TO PROCEED?[Y/N]: " << endl;
        cin >> p;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        p = static_cast<char>(toupper(p));
        if (p == 'Y') {
            flightData.erase(it);
            ofstream outputFile(fileName);
            if (outputFile.is_open()) {
                outputFile << flightData.dump(4) << endl;
                outputFile.close();
            } else {
                throw ios_base::failure("File couldn't be open");
            }
        } else {
            cout << "Thank you, BYE!";
            return false;
        }
    } else {
        cout << text1 << " with ID: " << flightId << ",couldn't be found!" << endl;
        return false;
    }
    return true;
}
bool Menu::validNumericData(const string& text, string& var) {
    int cnt1 = 0;
    bool res;
    do {
        cnt1++;
        cout << text << endl;
        cin >> var;
        res = regex_match(var, numericRegex);
        if (!res) {
            cout << "Please provide a numeric value.\n" << endl;
            if (cnt1 == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while (!res);
    return true;
}
bool Menu::validPlaneId(int id) {
    ifstream file("plane.json");
    nlohmann::json planeData;
    if (file.is_open()) {
        file >> planeData;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    bool res = false;
    for (auto& plane : planeData) {
        if (plane["id"] == id) {
            res = true;
            break;
        }
    }
    return res;
}
bool Menu::validRunwayId(int id, int& runwayDistance) {
    ifstream file("runway.json");
    nlohmann::json runwayData;
    if (file.is_open()) {
        file >> runwayData;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    bool res = false;
    for (auto& runway : runwayData) {
        if (runway["id"] == id) {
            runwayDistance = runway["distance"];
            res = true;
            break;
        }
    }
    return res;
}
void Menu::addFlightToFile(const Flight& flight) {
    nlohmann::json jsonFlight = flight.toJson();

    if (!filesystem::exists("flight.json")) {
        ofstream createFile("flight.json");
        if (createFile.is_open()) {
            createFile << "[]" << std::endl;
            createFile.close();
        } else {
            throw ios_base::failure("File couldn't be created");
        }
    }

    ifstream inputFile("flight.json");
    nlohmann::json existingData;

    if (inputFile.is_open()) {
        inputFile >> existingData;
        inputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    existingData.push_back(jsonFlight);

    ofstream outputFile("flight.json");

    if (outputFile.is_open()) {
        outputFile << existingData.dump(4) << endl;
        outputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}
void Menu::addRunwayToFile(const Runway& runway) {
    nlohmann::json jsonRunway = runway.toJson();

    if (!filesystem::exists("runway.json")) {
        ofstream createFile("runway.json");
        if (createFile.is_open()) {
            createFile << "[]" << std::endl;
            createFile.close();
        } else {
            throw ios_base::failure("File couldn't be created");
        }
    }

    ifstream inputFile("runway.json");
    nlohmann::json existingData;

    if (inputFile.is_open()) {
        inputFile >> existingData;
        inputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    existingData.push_back(jsonRunway);

    ofstream outputFile("runway.json");

    if (outputFile.is_open()) {
        outputFile << existingData.dump(4) << endl;
        outputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}
void Menu::userMenu() {
    char c;
    do {
        cout << "What do you want to do?\n"
                "Search for flights for specific destination[D].\n"
                "Search for planes by airline[A].\n"
                "If you want to exit the user menu press anything else[...]"<< endl;
        cin >> c;
        c = static_cast<char>(toupper(c));
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (c) {
            case 'D': {
                cout << "Enter starting destination: " << endl;
                string sDestination;
                cin >> sDestination;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter ending destination: " << endl;
                string eDestination;
                cin >> eDestination;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                searchByDestination(sDestination, eDestination);
                break;
            } case 'A': {
                cout << "Enter the airline: " << endl;
                string airline;
                cin >> airline;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                searchByAirline(airline);
                break;
            } default: {
                break;
            }
        }
        cout << "Do you want to continue searching[Y/N]: " << endl;
        cin >> c;
        c = static_cast<char>(toupper(c));
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while(c == 'Y');
}
void Menu::searchByAirline(const string& airline) {
    Plane::printPlane("airline", airline);
}
void Menu::searchByDestination(const string& sDestination, const string& eDestination) {
    ifstream file("flight.json");
    nlohmann::json flightData;
    if (file.is_open()) {
        file >> flightData;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    vector<shared_ptr<Flight>> flights;

    for (auto& flight : flightData) {
        if (flight["startingDestination"] == sDestination && flight["endingDestination"] == eDestination) {
            auto f = make_shared<Flight>();
            from_json(flight, *f);
            flights.push_back(f);
        }
    }
    printFlights(flights);
}
void Menu::printFlights(const vector<shared_ptr<Flight>>& flights) {
    for (const auto& flight : flights) {
        cout << "FLIGHT INFORMATION:" << endl;
        cout << *flight;
    }
}
int main() {
    int* ids = Menu::loadIds();
    Plane::setIdCnt(ids[0]);
    Flight::setIdCnt(ids[1]);
    Runway::setIdCnt(ids[2]);
    Menu::welcome();
    Menu::saveIds();
    delete[] ids;

    return 0;
}