#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <utility>
#include "json.hpp"
using namespace std;

enum Class {
    First, Second, Third
};

class ClassFactory {
public:
    static Class class_determination(const string& manufacturer, const string& model, double minRunwayLen);
};

enum PlaneType {
    PASSENGERS, BUSINESS, CARGO
};

PlaneType stringToEnum(const string&);

class PlaneClass {
private:
    string manufacturer;
    string model;
    double minRunwayDistance{};
    Class planeClass{};

public:
    PlaneClass(const string& manufacturer, const string& model, double minRunwayDistance)
        : manufacturer(manufacturer), model(model), minRunwayDistance(minRunwayDistance) {
        this->planeClass = ClassFactory::class_determination(manufacturer, model, minRunwayDistance);
    }
    PlaneClass() = default;
    ~PlaneClass() = default;
    [[nodiscard]] string getManufacturer();
    [[nodiscard]] string getModel();
    [[nodiscard]] double getMinRunwayDistance() const;
    [[nodiscard]] Class getPlaneClass() const;
    friend void from_json(const nlohmann::json& jsonPlane, PlaneClass& planeClass1) {
        jsonPlane.at("manufacturer").get_to(planeClass1.manufacturer);
        jsonPlane.at("model").get_to(planeClass1.model);
        jsonPlane.at("minRunwayDistance").get_to(planeClass1.minRunwayDistance);
        jsonPlane.at("planeClass").get_to(planeClass1.planeClass);

    }
    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json jsonPlane;
        jsonPlane["manufacturer"] = manufacturer;
        jsonPlane["model"] = model;
        jsonPlane["minRunwayDistance"] = minRunwayDistance;
        jsonPlane["planeClass"]= planeClass;
        return jsonPlane;
    }
    friend ostream& operator<<(ostream& out, const PlaneClass& planeClass1) {
        switch (planeClass1.getPlaneClass()) {
            case 0:
                out << "first";
                break;
            case 1:
                out << "second";
                break;
            case 2:
                out << "third";
                break;
        }
        return out;
    }
};

class Plane {
private:
    static int idCnt;
    int id{};
    PlaneClass planeClass;
    string airline;
    double oneKmCost{};
    double allCost{};
    int tankVolume{};
    double averageSpeed{};
public:
    Plane (const PlaneClass& planeClass, string  airline,double oneKmCost, double allCost, int tankVolume, double averageSpeed)
        : planeClass(planeClass), airline(std::move(airline)),oneKmCost(oneKmCost),
          allCost(allCost), tankVolume(tankVolume), averageSpeed(averageSpeed) {
        this->id = idCnt++;
    }
    Plane() = default;
    virtual ~Plane() = default;
    [[nodiscard]] int getId() const;
    [[nodiscard]] PlaneClass getPlaneClass() const;
    [[nodiscard]] string getAirline() const;
    [[nodiscard]] double getOneKmCost() const;
    [[nodiscard]] double getAllCost() const;
    [[nodiscard]] int getTankVolume() const;
    [[nodiscard]] double getAverageSpeed() const;
    static int getIdCnt();
    static void setIdCnt(int id);
    static void printPlane(const string&, const string&);
    void deserializeCommonData(const nlohmann::json& jsonPlane) {
        jsonPlane.at("id").get_to(id);
        jsonPlane.at("planeClass").get_to(planeClass);
        jsonPlane.at("airline").get_to(airline);
        jsonPlane.at("oneKmCost").get_to(oneKmCost);
        jsonPlane.at("allCost").get_to(allCost);
        jsonPlane.at("tankVolume").get_to(tankVolume);
        jsonPlane.at("averageSpeed").get_to(averageSpeed);
    }
    virtual PlaneType typeOfPlane() = 0;
    [[nodiscard]] virtual nlohmann::json toJson() const {
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
    friend ostream& operator<<(ostream& out, const Plane& plane) {
        out << "ID: " << plane.getId() << ", ";
        out << "Plane Class: " << plane.getPlaneClass() << ", ";
        out << "Airline: " << plane.getAirline() << ", ";
        out << "Cost for kilometer: " << plane.getOneKmCost() << ", ";
        out << "All cost: " << plane.getAllCost() << ", ";
        out << "Tank volume: " << plane.getTankVolume() << ", ";
        out << "Average speed: " << plane.getAverageSpeed() << ", ";
        return out;
    }
};

class PassengerPlane : public Plane {
private:
    int passengerSeats{};
    bool firstClass{};
public:
    PassengerPlane(const PlaneClass& plane_class, const string& airline, double one_km_cost, double all_cost, int tank_volume, double average_speed, int seats, bool first_class)
            : Plane(plane_class, airline, one_km_cost, all_cost, tank_volume, average_speed), passengerSeats(seats), firstClass(first_class){}
    PassengerPlane() = default;
    ~PassengerPlane() override = default;
    [[nodiscard]] int getPassengerSeats() const;
    [[nodiscard]] bool getFirstClass() const;
    PlaneType typeOfPlane() override;
    [[nodiscard]] nlohmann::json toJson() const override;
    friend void from_json(const nlohmann::json& json, PassengerPlane& passengerPlane) {
        passengerPlane.deserializeCommonData(json);
        json.at("passengerSeats").get_to(passengerPlane.passengerSeats);
        json.at("firstClass").get_to(passengerPlane.firstClass);
    }
    friend ostream& operator<<(ostream& out, const PassengerPlane& passengerPlane) {
        out << static_cast<const Plane&>(passengerPlane);

        out << "Passenger seats: " << passengerPlane.getPassengerSeats() << ", ";
        out << "First class: " << (passengerPlane.getFirstClass() ? "Yes" : "No") << ".\n";
        return out;
    }
};

class BusinessPlane : public Plane {
private:
    int passengerSeats{};
    bool flightEntertainment{};
    bool privateSuites{};

public:
    BusinessPlane(const PlaneClass& plane_class, const string& airline, double one_km_cost, double all_cost, int tank_volume, double average_speed, int seats, bool flightEntertainment, bool privateSuites)
            : Plane(plane_class, airline, one_km_cost, all_cost, tank_volume, average_speed), passengerSeats(seats), flightEntertainment(flightEntertainment), privateSuites(privateSuites){}
    BusinessPlane() = default;
    ~BusinessPlane() override = default;
    [[nodiscard]] int getPassengerSeats() const;
    [[nodiscard]] bool getFlightEntertainment() const;
    [[nodiscard]] bool getPrivateSuite() const;
    PlaneType typeOfPlane() override;
    [[nodiscard]] nlohmann::json toJson() const override;
    friend void from_json(const nlohmann::json& json, BusinessPlane& businessPlane) {
        businessPlane.deserializeCommonData(json);
        json.at("passengerSeats").get_to(businessPlane.passengerSeats);
        json.at("flightEntertainment").get_to(businessPlane.flightEntertainment);
        json.at("privateSuites").get_to(businessPlane.privateSuites);
    }
    friend ostream& operator<<(ostream& out, const BusinessPlane& businessPlane) {
        out << static_cast<const Plane&>(businessPlane);

        out << "Passenger seats: " << businessPlane.getPassengerSeats() << ", ";
        out << "Flight Entertainment: " << (businessPlane.getFlightEntertainment() ? "Yes" : "No") << ", ";
        out << "Private Suites: " << (businessPlane.getPrivateSuite() ? "Yes" : "No") << ".\n";
        return out;
    }
};

class CargoPlane : public Plane {
private:
    double capacity{};
    int numberOfCompartments{};
    bool temperatureControl{};

public:
    CargoPlane(const PlaneClass& plane_class, const string& airline, double one_km_cost, double all_cost, int tank_volume, double average_speed,
               double capacity, int numberOfCompartments, bool temperatureControl)
               : Plane(plane_class, airline, one_km_cost, all_cost, tank_volume, average_speed),
                 capacity(capacity), numberOfCompartments(numberOfCompartments), temperatureControl(temperatureControl){}
    CargoPlane() = default;
    ~CargoPlane() override = default;
    [[nodiscard]] double getCapacity() const;
    [[nodiscard]] int getNumberOfCompartments() const;
    [[nodiscard]] bool getTemperatureControl() const;
    PlaneType typeOfPlane() override;
    [[nodiscard]] nlohmann::json toJson() const override;
    friend void from_json(const nlohmann::json& json, CargoPlane& cargoPlane) {
        cargoPlane.deserializeCommonData(json);
        json.at("capacity").get_to(cargoPlane.capacity);
        json.at("numberOfCompartments").get_to(cargoPlane.numberOfCompartments);
        json.at("temperatureControl").get_to(cargoPlane.temperatureControl);
    }
    friend ostream& operator<<(ostream& out, const CargoPlane& cargoPlane) {
        out << static_cast<const Plane&>(cargoPlane);

        out << "Capacity: " << cargoPlane.getCapacity() << ", ";
        out << "Number of compartments: " << cargoPlane.getNumberOfCompartments() << ", ";
        out << "Temperature control: " << (cargoPlane.getTemperatureControl() ? "Yes" : "No") << ".\n";
        return out;
    }
};
#ifndef PLANE_H
#define PLANE_H

#endif //PLANE_H