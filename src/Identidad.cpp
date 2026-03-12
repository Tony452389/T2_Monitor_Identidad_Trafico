#include "monitor_red.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <chrono>

using namespace std;

string getMAC(const string& iface) {
    string path = "/sys/class/net/" + iface + "/address";
    ifstream file(path);

    string mac;
    if (file.is_open()) {
        getline(file, mac);
        file.close();
    }

    return mac;
}

string getIP(const string& iface) {

    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    string ip = "";

    if (getifaddrs(&ifaddr) == -1)
        return "";

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {

        if (ifa->ifa_addr == nullptr)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {

            if (iface == ifa->ifa_name) {

                int s = getnameinfo(
                    ifa->ifa_addr,
                    sizeof(struct sockaddr_in),
                    host,
                    NI_MAXHOST,
                    nullptr,
                    0,
                    NI_NUMERICHOST
                );

                if (s == 0)
                    ip = host;
            }
        }
    }

    freeifaddrs(ifaddr);
    return ip;
}

void logEvent(const string& msg) {

    ofstream log("network_identity.log", ios::app);

    auto now = chrono::system_clock::to_time_t(
        chrono::system_clock::now()
    );

    string time = ctime(&now);
    time.pop_back();

    string entry = "[" + time + "] " + msg;

    cout << entry << endl;

    if (log.is_open()) {
        log << entry << endl;
        log.close();
    }
}
