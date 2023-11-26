#include "validation.h"

bool Validations::validPlaneId(int id) {
    ifstream file("plane.json");
    nlohmann::json planeData;
    if (file.is_open()) {
        file >> planeData;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    bool res = false;
    for (auto& plane : planeData) {
        if (plane["id"] == id) {
            res = true;
            break;
        }
    }
    return res;
}
bool Validations::validRunwayId(int id, int& runwayDistance) {
    ifstream file("runway.json");
    nlohmann::json runwayData;
    if (file.is_open()) {
        file >> runwayData;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }

    bool res = false;
    for (auto& runway : runwayData) {
        if (runway["id"] == id) {
            runwayDistance = runway["distance"];
            res = true;
            break;
        }
    }
    return res;
}
bool Validations::validPlaneMinRunwayDistance(int planeId, int takeOffDistance, int landingDistance) {
    ifstream file("plane.json");
    nlohmann::json planes;
    if (file.is_open()) {
        file >> planes;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    for (auto plane : planes) {
        if (plane["id"] == planeId) {
            nlohmann::json planeClass = plane["planeClass"];
            double minDistance = planeClass["minRunwayDistance"];
            return minDistance <= takeOffDistance && minDistance <= landingDistance;
        }
    }
    return false;
}
bool Validations::validNumericData(const string& text, string& var) {
    int cnt1 = 0;
    bool res;
    do {
        cnt1++;
        cout << text << endl;
        getline(cin, var);
        res = regex_match(var, numericRegex);
        if (!res) {
            cout << "Please provide a numeric value.\n" << endl;
            if (cnt1 == 3) {
                cout << "Your session ended. Too many tries!\n" << endl;
                return false;
            }
        }
    } while (!res);
    return true;
}
bool Validations::credentialsInformation(string& username, string& password, bool isLogin) {
    cout << "Enter your username: " << endl;
    getline(cin, username);
    cout << "Enter your password: " << endl;
    getline(cin, password);
    return FilesHandling::isUsernameAvailable(username, password, isLogin);
}