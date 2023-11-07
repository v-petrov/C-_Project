#include "plane.h"
using namespace std;

Class ClassFactory::class_determination(const string &manufacturer, const string &model,
                                        double minRunwayLen) {
    if (minRunwayLen >= 3000) {
        return Class::First;
    } else if (minRunwayLen >= 2000) {
        return Class::Second;
    } else {
        return Class::Third;
    }
}

map<string, PlaneType> planeTypeDetermination = {
        {"PASSENGERS", PASSENGERS},
        {"BUSINESS", BUSINESS},
        {"CARGO", CARGO}
};


string PlaneClass::getManufacturer() {
    return this->manufacturer;
}

string PlaneClass::getModel() {
    return this->model;
}

double PlaneClass::getMinRunwayDistance() const {
    return this->minRunwayDistance;
}

Class PlaneClass::getPlaneClass() const {
    return this->planeClass;
}

int Plane::idCnt = 1;

void Plane::setIdCnt(int id) {
    idCnt = id;
}

int Plane::getIdCnt() {
    return idCnt;
}

int Plane::getId() const {
    return this->id;
}

PlaneClass Plane::getPlaneClass() const{
    return this->planeClass;
}

string Plane::getAirline() const {
    return this->airline;
}

double Plane::getOneKmCost() const {
    return this->oneKmCost;
}

double Plane::getAllCost() const {
    return this->allCost;
}
int Plane::getTankVolume() const {
    return this->tankVolume;
}
double Plane::getAverageSpeed() const {
    return this->averageSpeed;
}

void Plane::printPlane(const string& s, const string& s1) {
    ifstream file("plane.json");
    nlohmann::json jsonPlane;
    file >> jsonPlane;

    for (auto& plane : jsonPlane) {
        if (plane[s] == s1 || to_string(plane[s]) == s1) {
            if (plane.find("firstClass") != plane.end()) {
                PassengerPlane pp;
                from_json(plane, pp);
                cout << pp;
            } else if (plane.find("flightEntertainment") != plane.end()) {
                BusinessPlane bp;
                from_json(plane, bp);
                cout << bp;
            } else {
                CargoPlane cp;
                from_json(plane, cp);
                cout << cp;
            }
        }
    }
    file.close();
}

int PassengerPlane::getPassengerSeats() const {
    return this->passengerSeats;
}

bool PassengerPlane::getFirstClass() const {
    return this->firstClass;
}

PlaneType PassengerPlane::typeOfPlane() {
    return PlaneType::PASSENGERS;
}

nlohmann::json PassengerPlane::toJson() const {
    nlohmann::json jsonPlane = Plane::toJson();
    jsonPlane["passengerSeats"] = passengerSeats;
    jsonPlane["firstClass"] = firstClass;
    return jsonPlane;
}

PlaneType stringToEnum(const string& type) {
    auto planeType = planeTypeDetermination.find(type);
    if (planeType != planeTypeDetermination.end()) {
        return planeType->second;
    } else {
        return static_cast<PlaneType>(-1);
    }
}

int BusinessPlane::getPassengerSeats() const {
    return this->passengerSeats;
}

bool BusinessPlane::getFlightEntertainment() const {
    return this->flightEntertainment;
}

bool BusinessPlane::getPrivateSuite() const {
    return this->privateSuites;
}

PlaneType BusinessPlane::typeOfPlane() {
    return PlaneType::BUSINESS;
}

nlohmann::json BusinessPlane::toJson() const {
    nlohmann::json jsonPlane = Plane::toJson();
    jsonPlane["passengerSeats"] = passengerSeats;
    jsonPlane["flightEntertainment"] = flightEntertainment;
    jsonPlane["privateSuites"] = privateSuites;
    return jsonPlane;
}

double CargoPlane::getCapacity() const {
    return this->capacity;
}

int CargoPlane::getNumberOfCompartments() const {
    return this->numberOfCompartments;
}

bool CargoPlane::getTemperatureControl() const {
    return this->temperatureControl;
}

PlaneType CargoPlane::typeOfPlane() {
    return PlaneType::CARGO;
}

nlohmann::json CargoPlane::toJson() const {
    nlohmann::json jsonPlane = Plane::toJson();
    jsonPlane["capacity"] = capacity;
    jsonPlane["numberOfCompartments"] = numberOfCompartments;
    jsonPlane["temperatureControl"] = temperatureControl;
    return jsonPlane;
}