#ifndef JSONGEN_H
#define JSONGEN_H

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class JSONGen {
public:
    static void addEvent(const nlohmann::json& event);
    static void saveToFile(const std::string& filename);
private:
    static std::vector<nlohmann::json> events;
};

#endif
