#include "plane.h"
using namespace std;

nlohmann::json WriteToFile::toJson() const {
    return {};
}

Class ClassFactory::class_determination(const string &manufacturer) {
    if (manufacturer == "Boeing" || manufacturer == "Airbus" || manufacturer == "Embraer"|| manufacturer == "Airbus Canada") {
        return Class::A;
    } else if (manufacturer == "Comac" || manufacturer == "Sukhoi Civil" || manufacturer == "Mitsubishi"|| manufacturer == "ATR") {
        return Class::B;
    } else {
        return Class::C;
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

void from_json(const nlohmann::json& jsonPlane, PlaneClass& planeClass1) {
    jsonPlane.at("manufacturer").get_to(planeClass1.manufacturer);
    jsonPlane.at("model").get_to(planeClass1.model);
    jsonPlane.at("minRunwayDistance").get_to(planeClass1.minRunwayDistance);
    jsonPlane.at("planeClass").get_to(planeClass1.planeClass);
}

nlohmann::json PlaneClass::toJson() const {
    nlohmann::json jsonPlane;
    jsonPlane["manufacturer"] = manufacturer;
    jsonPlane["model"] = model;
    jsonPlane["minRunwayDistance"] = minRunwayDistance;
    jsonPlane["planeClass"]= planeClass;
    return jsonPlane;
}

ostream& operator<<(ostream& out, const PlaneClass& planeClass1) {
    switch (planeClass1.getPlaneClass()) {
        case 0:
            out << "A";
            break;
        case 1:
            out << "B";
            break;
        case 2:
            out << "C";
            break;
    }
    return out;
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

void Plane::deserializeCommonData(const nlohmann::json& jsonPlane) {
    jsonPlane.at("id").get_to(id);
    jsonPlane.at("planeClass").get_to(planeClass);
    jsonPlane.at("airline").get_to(airline);
    jsonPlane.at("oneKmCost").get_to(oneKmCost);
    jsonPlane.at("allCost").get_to(allCost);
    jsonPlane.at("tankVolume").get_to(tankVolume);
    jsonPlane.at("averageSpeed").get_to(averageSpeed);
}

nlohmann::json Plane::toJson() const {
    nlohmann::json jsonPlane;
    jsonPlane["id"] = id;
    jsonPlane["planeClass"] = planeClass.toJson();
    jsonPlane["airline"] = airline;
    jsonPlane["oneKmCost"] = oneKmCost;
    jsonPlane["allCost"] = allCost;
    jsonPlane["tankVolume"] = tankVolume;
    jsonPlane["averageSpeed"] = averageSpeed;
    return jsonPlane;
}

ostream& operator<<(ostream& out, const Plane& plane) {
    out << "ID: " << plane.getId() << ", ";
    out << "Plane Class: " << plane.getPlaneClass() << ", ";
    out << "Airline: " << plane.getAirline() << ", ";
    out << "Cost for kilometer: " << plane.getOneKmCost() << ", ";
    out << "All cost: " << plane.getAllCost() << ", ";
    out << "Tank volume: " << plane.getTankVolume() << ", ";
    out << "Average speed: " << plane.getAverageSpeed() << ", ";
    return out;
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

void from_json(const nlohmann::json& json, PassengerPlane& passengerPlane) {
    passengerPlane.deserializeCommonData(json);
    json.at("passengerSeats").get_to(passengerPlane.passengerSeats);
    json.at("firstClass").get_to(passengerPlane.firstClass);
}

ostream& operator<<(ostream& out, const PassengerPlane& passengerPlane) {
    out << static_cast<const Plane&>(passengerPlane);

    out << "Passenger seats: " << passengerPlane.getPassengerSeats() << ", ";
    out << "First class: " << (passengerPlane.getFirstClass() ? "Yes" : "No") << ".\n";
    return out;
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

void from_json(const nlohmann::json& json, BusinessPlane& businessPlane) {
    businessPlane.deserializeCommonData(json);
    json.at("passengerSeats").get_to(businessPlane.passengerSeats);
    json.at("flightEntertainment").get_to(businessPlane.flightEntertainment);
    json.at("privateSuites").get_to(businessPlane.privateSuites);
}

ostream& operator<<(ostream& out, const BusinessPlane& businessPlane) {
    out << static_cast<const Plane&>(businessPlane);

    out << "Passenger seats: " << businessPlane.getPassengerSeats() << ", ";
    out << "Flight Entertainment: " << (businessPlane.getFlightEntertainment() ? "Yes" : "No") << ", ";
    out << "Private Suites: " << (businessPlane.getPrivateSuite() ? "Yes" : "No") << ".\n";
    return out;
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

void from_json(const nlohmann::json& json, CargoPlane& cargoPlane) {
    cargoPlane.deserializeCommonData(json);
    json.at("capacity").get_to(cargoPlane.capacity);
    json.at("numberOfCompartments").get_to(cargoPlane.numberOfCompartments);
    json.at("temperatureControl").get_to(cargoPlane.temperatureControl);
}

ostream& operator<<(ostream& out, const CargoPlane& cargoPlane) {
    out << static_cast<const Plane&>(cargoPlane);

    out << "Capacity: " << cargoPlane.getCapacity() << ", ";
    out << "Number of compartments: " << cargoPlane.getNumberOfCompartments() << ", ";
    out << "Temperature control: " << (cargoPlane.getTemperatureControl() ? "Yes" : "No") << ".\n";
    return out;
}