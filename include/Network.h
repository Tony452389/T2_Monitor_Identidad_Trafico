#pragma once

#include <string>

struct Interface
{
    std::string name;
    std::string ipv4;
};

Interface getActiveInterface();
