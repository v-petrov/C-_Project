#include "objects.h"
#include "validation.h"

regex numericRegex("^[0-9]+(\\.[0-9]+)?$");
regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
regex timeRegex("\\d{2}:\\d{2}");

bool ObjectsManaging::createPlane() {
    string manufacturer, model, minRunwayDistance;
    cnt = 0;

    cout << "Enter plain's manufacturer: " << endl;
    getline(cin, manufacturer);
    cout << "Enter plain's model: " << endl;
    getline(cin, model);
    if (!Validations::validNumericData("Enter plain's minimum runway distance: ", minRunwayDistance)) {
        return false;
    }
    PlaneClass planeClass(manufacturer, model, stod(minRunwayDistance));

    string airline, oneKmCost, tankVolume, allCost, averageSpeed;

    cout << "Enter plain's airline: " << endl;
    getline(cin, airline);
    if (!Validations::validNumericData("Enter plain's one kilometer cost: ", oneKmCost)) {
        return false;
    }
    if (!Validations::validNumericData("Enter plain's total cost: ", allCost)) {
        return false;
    }
    if (!Validations::validNumericData("Enter plain's tank volume: ", tankVolume)) {
        return false;
    }
    if (!Validations::validNumericData("Enter plain's average speed: ", averageSpeed)) {
        return false;
    }
    char c = 't';

    while (true) {
        cnt++;
        cout << "What type of plane do you want to add[PASSENGERS, BUSINESS, CARGO]: " << endl;
        string planeType;
        getline(cin, planeType);
        transform(planeType.begin(), planeType.end(), planeType.begin(), ::toupper);
        PlaneType pt = stringToEnum(planeType);

        switch (pt) {
            case PASSENGERS: {
                string firstClass, passengerSeats;
                if (!Validations::validNumericData("Enter plain's passenger seats: ", passengerSeats)) {
                    return false;
                }
                cout << "Enter if the plain has first class[TRUE/FALSE]: " << endl;
                getline(cin, firstClass);
                bool firstClassBool = (firstClass == "TRUE" || firstClass == "true");

                PassengerPlane passengerPlane(planeClass, airline, stod(oneKmCost), stod(allCost), stoi(tankVolume), stod(averageSpeed), stoi(passengerSeats), firstClassBool);
                const Plane& plane = passengerPlane;
                FilesHandling::addPlaneToFile(plane);
                break;
            } case BUSINESS: {
                string passengerSeats, flightEntertainment, privateSuites;
                if (!Validations::validNumericData("Enter plain's passenger seats: ", passengerSeats)) {
                    return false;
                }
                cout << "Enter if the plain has flight entertainment[TRUE/FALSE]: " << endl;
                getline(cin, flightEntertainment);
                bool flightEntertainmentBool = (flightEntertainment == "TRUE" || flightEntertainment == "true");

                cout << "Enter if plain has private suites[TRUE/FALSE]: " << endl;
                getline(cin, privateSuites);
                bool privateSuitesBool = (privateSuites == "TRUE" || privateSuites == "true");

                BusinessPlane businessPlane(planeClass, airline, stod(oneKmCost), stod(allCost), stoi(tankVolume), stod(averageSpeed), stoi(passengerSeats), flightEntertainmentBool, privateSuitesBool);
                const Plane& plane = businessPlane;
                FilesHandling::addPlaneToFile(plane);
                break;
            } case CARGO: {
                string capacity, numberOfCompartments, temperatureControl;
                if (!Validations::validNumericData("Enter plain's capacity: ", capacity)) {
                    return false;
                }
                if (!Validations::validNumericData("Enter plain's number of compartments: ", numberOfCompartments)) {
                    return false;
                }
                cout << "Enter if plain has temperature control[TRUE/FALSE]: " << endl;
                getline(cin, temperatureControl);
                bool temperatureControlBool = (temperatureControl == "TRUE" || temperatureControl == "true");

                CargoPlane cargoPlane(planeClass, airline, stod(oneKmCost), stod(allCost), stoi(tankVolume), stod(averageSpeed), stod(capacity), stoi(numberOfCompartments), temperatureControlBool);
                const Plane& plane = cargoPlane;
                FilesHandling::addPlaneToFile(plane);
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
bool ObjectsManaging::createRunway() {
    string airportName, distance;
    cout << "Enter runways airport name: " << endl;
    getline(cin, airportName);
    if (!Validations::validNumericData("Enter runways distance: ", distance)) {
        return false;
    }
    Runway runway(airportName, stoi(distance));
    FilesHandling::addRunwayToFile(runway);
    return true;
}
bool ObjectsManaging::createFlight() {
    string flightStatus, startingDestination, endingDestination, totalDistance;
    FlightStatus flightStatusEnum;
    bool res;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter flights status[FINISHED, ONGOING, INCOMING]: " << endl;
        getline(cin, flightStatus);
        transform(flightStatus.begin(), flightStatus.end(), flightStatus.begin(), ::toupper);
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
    getline(cin, startingDestination);
    int takeOffRunwayId;
    int takeOffRunwayDistance;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter take off runway id for the flight: " << endl;
        cin >> takeOffRunwayId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        res = Validations::validRunwayId(takeOffRunwayId, takeOffRunwayDistance);
        if (!res) {
            cout << "Please provide a correct runway's id for the flight.\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while(!res);
    cout << "Enter flights ending destination: " << endl;
    getline(cin, endingDestination);
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
        res = Validations::validRunwayId(landingRunwayId, landingRunwayDistance);
        if (!res) {
            cout << "Please provide a correct runway's id for the flight.\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while(!res);
    if (!Validations::validNumericData("Enter flights total distance(km): ", totalDistance)) {
        return false;
    }

    string dateInput, timeInput;
    cnt = 0;
    do {
        cnt++;
        cout << "Enter date of the flight(YYYY-MM-DD): " << endl;
        getline(cin, dateInput);
        cout << "Enter time of the flight(HH:MM): " << endl;
        getline(cin, timeInput);
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
        res = Validations::validPlaneId(planeId);
        if (!res) {
            cout << "Please provide a correct plane's id for the flight.\n" << endl;
            if (cnt == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        } else {
            if (!Validations::validPlaneMinRunwayDistance(planeId, takeOffRunwayDistance, landingRunwayDistance)) {
                cout << "Plane's minimal runway distance is longer than one or both of the runways! Please choose different plane." << endl;
                res = false;
            }
        }
    } while(!res);
    Flight flight(flightStatusEnum, startingDestination, takeOffRunwayId, endingDestination, landingRunwayId, stod(totalDistance), dateInput, timeInput, planeId);
    FilesHandling::addFlightToFile(flight);
    return true;
}
bool ObjectsManaging::deleteObject(const string& text, const string& fileName, const string& text1, int& id) {
    string flightId;
    if (!Validations::validNumericData(text, flightId)) {
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
bool ObjectsManaging::deletePlane(const string& text, const string& fileName, const string& text1) {
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
void ObjectsManaging::searchByAirline(const string& airline) {
    Plane::printPlane("airline", airline);
}
void ObjectsManaging::searchByDestination(const string& sDestination, const string& eDestination) {
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
void ObjectsManaging::printFlights(const vector<shared_ptr<Flight>>& flights) {
    for (const auto& flight : flights) {
        cout << "FLIGHT INFORMATION:" << endl;
        cout << *flight;
    }
}