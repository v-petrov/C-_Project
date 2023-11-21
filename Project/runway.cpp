#include "runway.h"

int Runway::idCnt = 1;

void Runway::setIdCnt(int i) {
    idCnt = i;
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
    out << "Airport name: " << runway.getAirportName();
    out << "Runway ID: " << runway.getId() << ", ";
    out << "Distance: " << runway.getDistance();
    return out;
}