#pragma once

#include <string>
#include <queue>
#include "Evento.h"   // Define struct Evento y enum TipoEvento

// Funciones auxiliares
std::string getMAC(const std::string& iface);
std::string getIP(const std::string& iface);
std::string getTimestamp();
void logEvent(const std::string& msg);

// Función principal de monitoreo
void iniciarIdentidad();

// Cola global de eventos (debe definirse en otro módulo)
extern std::queue<Evento> queueEntrada;