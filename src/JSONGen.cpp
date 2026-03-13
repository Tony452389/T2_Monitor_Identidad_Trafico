#include "JSONGen.h"
#include <fstream>

std::vector<nlohmann::json> JSONGen::events;

void JSONGen::addEvent(const nlohmann::json& event) {
    events.push_back(event);
}

void JSONGen::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << nlohmann::json(events).dump(4);
    }
}
