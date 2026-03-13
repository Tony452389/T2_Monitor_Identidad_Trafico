//Hecho por: Sergio Pedro Sepúlveda Rodríguez
#pragma once
#include <string>
#include <unordered_map>

// Declaración de la cola global
#include "EventQueue.h"
extern EventQueue queueEntrada;

// Estructura para almacenar estado previo de la interfaz
struct EstadoIdentidad {
    std::string ip;
    std::string mac;
};

class ModuloIdentidad {
public:
    ModuloIdentidad() = default;

    // Procesa el estado actual de la interfaz y genera eventos si hay cambios
    void procesarEstadoActual(const std::string& iface, const std::string& ip, const std::string& mac);

private:
    std::unordered_map<std::string, EstadoIdentidad> estadosPrevios;

    // Genera un evento de cambio de IP o MAC
    void generarEventoCambio(const std::string& iface,
                             const EstadoIdentidad& anterior,
                             const EstadoIdentidad& actual);
};