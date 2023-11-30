#include "runway.h"

int Runway::idCnt = 1;

void Runway::setIdCnt(int i) {
    idCnt = i;
}
int Runway::getIdCnt() {
    return idCnt;
}
int Runway::getId() const {
    return this->id;
}
string Runway::getAirportName() const {
    return this->airportName;
}
int Runway::getDistance() const {
    return this->distance;
}

void Runway::printRunway(int id) {
    ifstream file("runway.json");
    nlohmann::json jsonRunway;
    file >> jsonRunway;

    for (auto& runway : jsonRunway) {
        if (runway["id"] == id) {
            Runway runway1;
            from_json(runway, runway1);
            cout << runway1;
        }
    }
}
nlohmann::json Runway::toJson() const {
    nlohmann::json jsonRunway;
    jsonRunway["id"] = id;
    jsonRunway["airportName"] = airportName;
    jsonRunway["distance"] = distance;
    return jsonRunway;
}
void from_json(const nlohmann::json& json, Runway& runway) {
    json.at("id").get_to(runway.id);
    json.at("airportName").get_to(runway.airportName);
    json.at("distance").get_to(runway.distance);
}
ostream& operator<<(ostream & out, const Runway& runway) {
    out << "Airport name: " << runway.getAirportName() << ", ";
    out << "Runway ID: " << runway.getId() << ", ";
    out << "Distance: " << runway.getDistance() << ".\n";
    return out;
}