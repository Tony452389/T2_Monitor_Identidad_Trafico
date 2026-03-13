#pragma once
#include <string>
#include <unordered_map>
#include "eventqueue.h"
#include "evento.h"

class ModuloIdentidad {
public:
    // Constructor recibe la cola de eventos thread-safe
    explicit ModuloIdentidad(EventQueue& colaEventos);

    // Procesa el estado actual de la interfaz y genera eventos si hay cambios
    void procesarEstadoActual(const std::string& iface, const std::string& ip, const std::string& mac);

private:
    EventQueue& queueEntrada;  // referencia a la cola thread-safe

    struct EstadoIdentidad {
        std::string ip;
        std::string mac;
    };

    std::unordered_map<std::string, EstadoIdentidad> estadosPrevios;

    // Método privado para generar eventos de cambio de identidad
    void generarEventoCambio(const std::string& iface, const EstadoIdentidad& anterior, const EstadoIdentidad& actual);
};