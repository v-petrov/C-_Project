#include <utility>

#include "plane.h"
using namespace std;

enum FlightStatus {
    FINISHED, ONGOING, INCOMING, ERROR
};

FlightStatus stringToEnumFlight(const string&);

class Flight {
private:
    static int idCnt;
    int id{};
    FlightStatus status{};
    string startingDestination;
    string endingDestination;
    double totalDistance{};
    string date;
    string time;
    int plainId{};
public:
    Flight(FlightStatus status, string  startingDestination, string  endingDestination,
           double totalDistance, string  date, string  time, int plainId)
           : status(status), startingDestination(std::move(startingDestination)), endingDestination(std::move(endingDestination)), totalDistance(totalDistance),
           date(std::move(date)), time(std::move(time)), plainId(plainId) {
        this->id = idCnt++;
    }
    Flight() = default;
    ~Flight() = default;
    [[nodiscard]] int getId() const;
    FlightStatus getFlightStatus();
    string getStartingDestination();
    string getEndingDestination();
    [[nodiscard]] double getTotalDistance() const;
    string getDate();
    string getTime();
    [[nodiscard]] int getPlaneId() const;
    static int getIdCnt();
    static void setIdCnt(int id);
    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json jsonFlight;
        jsonFlight["id"] = id;
        jsonFlight["status"] = status;
        jsonFlight["startingDestination"] = startingDestination;
        jsonFlight["endingDestination"] = endingDestination;
        jsonFlight["totalDistance"] = totalDistance;
        jsonFlight["date"] = date;
        jsonFlight["time"] = time;
        jsonFlight["plainId"] = plainId;
        return jsonFlight;
    }
    friend void from_json(const nlohmann::json& json, Flight& flight) {
        json.at("id").get_to(flight.id);
        json.at("status").get_to(flight.status);
        json.at("startingDestination").get_to(flight.startingDestination);
        json.at("endingDestination").get_to(flight.endingDestination);
        json.at("totalDistance").get_to(flight.totalDistance);
        json.at("date").get_to(flight.date);
        json.at("time").get_to(flight.time);
        json.at("plainId").get_to(flight.plainId);
    }
    friend ostream& operator<<(ostream& out, Flight flight) {
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
        out << "Ending Destination: " << flight.getEndingDestination() << ", ";
        out << "Total distance(km): " << flight.getTotalDistance() << ", ";
        out << "Date: " << flight.getDate() << ", ";
        out << "Time: " << flight.getTime() << ".\n";
        out << "PLANE INFORMATION: \n";
        Plane::printPlane("id", to_string(flight.getPlaneId()));
        return out;
    }
};
#ifndef PROJECT_FLIGHT_H
#define PROJECT_FLIGHT_H

#endif //PROJECT_FLIGHT_H
