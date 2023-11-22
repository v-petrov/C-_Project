#include <utility>

#include "plane.h"
#include "runway.h"
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
    int takeoffRunwayId{};
    string endingDestination;
    int landingRunwayId{};
    double totalDistance{};
    string date;
    string time;
    int plainId{};
public:
    Flight(FlightStatus status, string  startingDestination, int takeoffRunwayId, string  endingDestination,
           int landingRunwayId, double totalDistance, string  date, string  time, int plainId)
           : status(status), startingDestination(std::move(startingDestination)), takeoffRunwayId(takeoffRunwayId),
           landingRunwayId(landingRunwayId), endingDestination(std::move(endingDestination)), totalDistance(totalDistance),
           date(std::move(date)), time(std::move(time)), plainId(plainId) {
        this->id = idCnt++;
    }
    Flight() = default;
    ~Flight() = default;
    [[nodiscard]] int getId() const;
    FlightStatus getFlightStatus();
    string getStartingDestination();
    [[nodiscard]] int getTakeOffRunwayId() const;
    string getEndingDestination();
    [[nodiscard]] int getLandingRunwayId() const;
    [[nodiscard]] double getTotalDistance() const;
    string getDate();
    string getTime();
    [[nodiscard]] int getPlaneId() const;
    static int getIdCnt();
    static void setIdCnt(int id);
    [[nodiscard]] nlohmann::json toJson() const;
    friend void from_json(const nlohmann::json& json, Flight& flight);
    friend ostream& operator<<(ostream& out, Flight flight);
};
#ifndef PROJECT_FLIGHT_H
#define PROJECT_FLIGHT_H

#endif //PROJECT_FLIGHT_H
