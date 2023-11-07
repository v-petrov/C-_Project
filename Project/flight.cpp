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

string Flight::getEndingDestination() {
    return this->endingDestination;
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