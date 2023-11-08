#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <utility>
#include "json.hpp"
using namespace std;

enum Class {
    A, B, C
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
    friend void from_json(const nlohmann::json& jsonPlane, PlaneClass& planeClass1);
    [[nodiscard]] nlohmann::json toJson() const;
    friend ostream& operator<<(ostream& out, const PlaneClass& planeClass1);
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
    void deserializeCommonData(const nlohmann::json& jsonPlane);
    virtual PlaneType typeOfPlane() = 0;
    [[nodiscard]] virtual nlohmann::json toJson() const;
    friend ostream& operator<<(ostream& out, const Plane& plane);
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
    friend void from_json(const nlohmann::json& json, PassengerPlane& passengerPlane);
    friend ostream& operator<<(ostream& out, const PassengerPlane& passengerPlane);
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
    friend void from_json(const nlohmann::json& json, BusinessPlane& businessPlane);
    friend ostream& operator<<(ostream& out, const BusinessPlane& businessPlane);
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
    friend void from_json(const nlohmann::json& json, CargoPlane& cargoPlane);
    friend ostream& operator<<(ostream& out, const CargoPlane& cargoPlane);
};
#ifndef PLANE_H
#define PLANE_H

#endif //PLANE_H