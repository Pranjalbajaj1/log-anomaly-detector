#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>

using namespace std;

string getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    char buffer[20];
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);

    return string(buffer);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Usage: generator <normal|spike|silence>\n";
        return 1;
    }

    string mode = argv[1];

    ofstream logFile("logs/app.log", ios::app);
    if (!logFile.is_open()) {
        cout << "Error: could not open log file\n";
        return 1;
    }

    srand(time(nullptr));

    while (true) {

        if (mode == "silence") {
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        string service = (rand() % 2 == 0) ? "AuthService" : "PaymentService";

        string level;
        if (mode == "normal") {
            level = (rand() % 100 < 95) ? "INFO" : "ERROR";
        } else {
            level = (rand() % 100 < 90) ? "ERROR" : "INFO";
        }

        string message = (level == "ERROR") ?
                         "Operation failed" :
                         "Operation successful";

        //log formatting
        logFile << getCurrentTimestamp() << " "
                << level << " "
                << service << " "
                << message << endl;

        logFile.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
