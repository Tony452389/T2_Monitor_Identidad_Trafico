/*
-----------------------------------------------------------------------------------
Modulo: Identidad
Encargado del modulo: Sergio Pedro Sepulveda Rodriguez
Proyecto: T2 - Monitor de Identidad y Trafico
-----------------------------------------------------------------------------------
*/

#include "Identidad.h"
#include "Evento.h"
#include <chrono>
#include <ctime>

// Procesa el estado actual de la interfaz
void ModuloIdentidad::procesarEstadoActual(const std::string& iface, const std::string& ip, const std::string& mac) {
    EstadoIdentidad actual{ip, mac};

    auto it = estadosPrevios.find(iface);
    if (it != estadosPrevios.end()) {
        EstadoIdentidad& previo = it->second;
        if (previo.ip != actual.ip || previo.mac != actual.mac) {
            generarEventoCambio(iface, previo, actual);
        }
    }

    // Actualiza el estado previo de la interfaz
    estadosPrevios[iface] = actual;
}

// Genera un evento de cambio de IP o MAC y lo envía a la cola global
void ModuloIdentidad::generarEventoCambio(const std::string& iface,
                                           const EstadoIdentidad& anterior,
                                           const EstadoIdentidad& actual)
{
    Evento e;

    // Tipo de evento
    if (anterior.ip != actual.ip) {
        e.tipo = TipoEvento::IP_CHANGE;
    } else if (anterior.mac != actual.mac) {
        e.tipo = TipoEvento::MAC_CHANGE;
    } else {
        e.tipo = TipoEvento::UNKNOWN;
    }

    // Módulo que genera el evento
    e.origenModulo = "ModuloIdentidad";

    // Descripción del cambio
    e.descripcion = "Interfaz " + iface + " cambio: IP " + anterior.ip + " -> " + actual.ip +
                    ", MAC " + anterior.mac + " -> " + actual.mac;

    // Valores actuales
    e.ipOrigen = actual.ip;
    e.macOrigen = actual.mac;

    // Timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    e.timestamp = std::ctime(&t);
    e.timestamp.pop_back(); // eliminar salto de línea

    // Nivel de riesgo
    e.nivelRiesgo = NivelRiesgo::NONE;

    // Enviar evento a la cola global thread-safe
    queueEntrada.push(e);
}