#include "files.h"

int* FilesHandling::loadIds() {
    int* idPointer = new int[3];
    ifstream file("ids.txt");
    if (file.is_open()) {
        string line;
        getline(file, line);
        size_t pos = line.find(',');
        if (pos != string::npos) {
            string planeIds = line.substr(0, pos);
            string flightIds = line.substr(pos + 1);
            string runwayIds = line.substr(pos + 3);
            idPointer[0] = stoi(planeIds);
            idPointer[1] = stoi(flightIds);
            idPointer[2] = stoi(runwayIds);
        }
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    return idPointer;
}
void FilesHandling::saveIds(const int* ids) {
    ofstream file("ids.txt");
    if (file.is_open()) {
        file << Plane::getIdCnt() << "," << Flight::getIdCnt() << "," << Runway::getIdCnt() << endl;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    delete[] ids;
}
int* FilesHandling::setIds() {
    int* ids = loadIds();
    Plane::setIdCnt(ids[0]);
    Flight::setIdCnt(ids[1]);
    Runway::setIdCnt(ids[2]);
    return ids;
}
bool FilesHandling::isUsernameAvailable(const string& username, const string& password, bool isLogin) {
    ifstream file("admin.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string currUsername = line.substr(0, pos);
                if (isLogin) {
                    string currPassword = line.substr(pos + 1);
                    if (currUsername == username && currPassword == password) {
                        file.close();
                        return true;
                    }
                } else {
                    if (currUsername == username) {
                        file.close();
                        return false;
                    }
                }
            }
        }
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
    return !isLogin;
}
void FilesHandling::saveUser(const string &username, const string &password) {
    ofstream file("admin.txt", ios::app);
    if (file.is_open()) {
        file << username << "," << password << endl;
        file.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}
void FilesHandling::addDataToFile(const WriteToFile& data, const string& fileName) {
    nlohmann::json jsonData = data.toJson();

    if (!filesystem::exists(fileName)) {
        ofstream createFile(fileName);
        if (createFile.is_open()) {
            createFile << "[]" << std::endl;
            createFile.close();
        } else {
            throw ios_base::failure("File couldn't be created");
        }
    }

    ifstream inputFile(fileName);
    nlohmann::json existingData;

    if (inputFile.is_open()) {
        inputFile >> existingData;
        inputFile.close();
    }
    else {
        throw ios_base::failure("File couldn't be open");
    }

    existingData.push_back(jsonData);

    ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        outputFile << existingData.dump(4) << endl;
        outputFile.close();
    } else {
        throw ios_base::failure("File couldn't be open");
    }
}