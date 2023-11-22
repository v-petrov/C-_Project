#include "flight.h"
using namespace std;

map<string, FlightStatus> flightStatusDetermination = {
        {"FINISHED", FINISHED},
        {"ONGOING", ONGOING},
        {"INCOMING", INCOMING}
};

FlightStatus stringToEnumFlight(const string& type) {
    auto planeType = flightStatusDetermination.find(type);
    if (planeType != flightStatusDetermination.end()) {
        return planeType->second;
    } else {
        return FlightStatus::ERROR;
    }
}

int Flight::idCnt = 1;

void Flight::setIdCnt(int id) {
    idCnt = id;
}

int Flight::getIdCnt() {
    return idCnt;
}

int Flight::getId() const {
    return this->id;
}

FlightStatus Flight::getFlightStatus() {
    return this->status;
}

string Flight::getStartingDestination() {
    return this->startingDestination;
}

int Flight::getTakeOffRunwayId() const {
    return this->takeoffRunwayId;
}

string Flight::getEndingDestination() {
    return this->endingDestination;
}

int Flight::getLandingRunwayId() const {
    return this->landingRunwayId;
}

double Flight::getTotalDistance() const {
    return this->totalDistance;
}

string Flight::getDate() {
    return date;
}

string Flight::getTime() {
    return this->time;
}

int Flight::getPlaneId() const {
    return this->plainId;
}

nlohmann::json Flight::toJson() const {
    nlohmann::json jsonFlight;
    jsonFlight["id"] = id;
    jsonFlight["status"] = status;
    jsonFlight["startingDestination"] = startingDestination;
    jsonFlight["takeoffRunwayId"] = takeoffRunwayId;
    jsonFlight["endingDestination"] = endingDestination;
    jsonFlight["landingRunwayId"] = landingRunwayId;
    jsonFlight["totalDistance"] = totalDistance;
    jsonFlight["date"] = date;
    jsonFlight["time"] = time;
    jsonFlight["plainId"] = plainId;
    return jsonFlight;
}

void from_json(const nlohmann::json& json, Flight& flight) {
    json.at("id").get_to(flight.id);
    json.at("status").get_to(flight.status);
    json.at("startingDestination").get_to(flight.startingDestination);
    json.at("takeoffRunwayId").get_to(flight.takeoffRunwayId);
    json.at("endingDestination").get_to(flight.endingDestination);
    json.at("landingRunwayId").get_to(flight.landingRunwayId);
    json.at("totalDistance").get_to(flight.totalDistance);
    json.at("date").get_to(flight.date);
    json.at("time").get_to(flight.time);
    json.at("plainId").get_to(flight.plainId);
}

ostream& operator<<(ostream& out, Flight flight) {
    out << "ID: " << flight.getId() << ", ";
    out << "Status: ";
    switch (flight.getFlightStatus()) {
        case 0:
            out << "finished" << ", ";
            break;
        case 1:
            out << "ongoing" << ", ";
            break;
        case 2:
            out << "incoming" << ", ";
            break;
        case 3:
            break;
    }
    out << "Starting Destination: " << flight.getStartingDestination() << ", ";
    Runway::printRunway(flight.getTakeOffRunwayId());
    out << "Ending Destination: " << flight.getEndingDestination() << ", ";
    Runway::printRunway(flight.getLandingRunwayId());
    out << "Total distance(km): " << flight.getTotalDistance() << ", ";
    out << "Date: " << flight.getDate() << ", ";
    out << "Time: " << flight.getTime() << ".\n";
    out << "PLANE INFORMATION: \n";
    Plane::printPlane("id", to_string(flight.getPlaneId()));
    return out;
}