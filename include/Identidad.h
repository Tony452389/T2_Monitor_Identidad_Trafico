#ifndef MONITOR_RED_H
#define MONITOR_RED_H

#include <string>

std::string getMAC(const std::string& iface);
std::string getIP(const std::string& iface);
void logEvent(const std::string& msg);

#endif
