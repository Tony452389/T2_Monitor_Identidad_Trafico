#include "Identidad.h"

#include <fstream>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

// Obtiene la MAC de la interfaz
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

// Obtiene la IP IPv4 de la interfaz
string getIP(const string& iface) {
    struct ifaddrs *ifaddr, *ifa;
    string ip = "";

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return "";
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;

        if (ifa->ifa_addr->sa_family == AF_INET &&
            iface == ifa->ifa_name) {

            char host[NI_MAXHOST];
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &sa->sin_addr, host, NI_MAXHOST);
            ip = host;
        }
    }

    freeifaddrs(ifaddr);
    return ip;
}

// Obtiene timestamp como string
string getTimestamp() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

// Log local
void logEvent(const string& msg) {
    ofstream log("network_identity_monitor.log", ios::app);
    log << getTimestamp() << " - " << msg << endl;
    log.close();
}

// Función principal de monitoreo de identidad
void iniciarIdentidad() {
    string iface;
    cout << "Ingrese la interfaz a monitorear: ";
    cin >> iface;

    string lastMAC = getMAC(iface);
    string lastIP = getIP(iface);

    logEvent("Inicio de monitoreo - IP: " + lastIP + " MAC: " + lastMAC);

    while (true) {
        string currentMAC = getMAC(iface);
        string currentIP = getIP(iface);

        // Detecta cambio de MAC
        if (currentMAC != lastMAC) {
            Evento e;
            e.tipo = MAC_CHANGE;
            e.iface = iface;
            e.valorAnterior = lastMAC;
            e.valorActual = currentMAC;
            e.timestamp = getTimestamp();
            queueEntrada.push(e);  // envia al flujo del sistema

            logEvent("MAC cambiada: " + lastMAC + " -> " + currentMAC);
            lastMAC = currentMAC;
        }

        // Detecta cambio de IP
        if (currentIP != lastIP) {
            Evento e;
            e.tipo = IP_CHANGE;
            e.iface = iface;
            e.valorAnterior = lastIP;
            e.valorActual = currentIP;
            e.timestamp = getTimestamp();
            queueEntrada.push(e);  // envia al flujo del sistema

            logEvent("IP cambiada: " + lastIP + " -> " + currentIP);
            lastIP = currentIP;
        }

        sleep(5); // intervalo de monitoreo
    }
}