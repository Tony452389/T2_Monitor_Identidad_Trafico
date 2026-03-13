#include <iostream>

#include "Evento.h"
#include "EventQueue.h"

#include "Analisis.h"
#include "Identidad.h"
#include "Sniffer.h"
#include "JSONGen.h"

#include <thread>

EventQueue queueEntrada;
EventQueue queueSalida;

int main()
{
    std::cout << "Sistema de monitoreo iniciado" << std::endl;

    iniciarIdentidad();
    iniciarSniffer();

    //----------------- Bloque de codigo temporal para añadir eventos a la cola y hacer pruebas con el modulo de Analisis ----------------------------
    std::thread tAnalisis(iniciarAnalisis);

    for(int i = 0; i <6; i++){
        Evento e1;
         e1.tipo = TipoEvento::ARP;
        e1.descripcion = "ARP de prueba";
        e1.origenModulo = "TEST";
        queueEntrada.push(e1);

        Evento e2;
        e2.tipo = TipoEvento::ICMP;
        e2.descripcion = "ICMP de prueba";
        e2.origenModulo = "TEST";
        queueEntrada.push(e2);

        Evento e3;
        e3.tipo = TipoEvento::IP_CHANGE;
        e3.descripcion = "Cambio de IP de prueba";
        e3.origenModulo = "TEST";
        queueEntrada.push(e3);

        Evento e4;
        e4.tipo = TipoEvento::MAC_CHANGE;
        e4.descripcion = "Cambio de MAC de prueba";
        e4.origenModulo = "TEST";
        queueEntrada.push(e4);

        Evento e5;
        e5.tipo = TipoEvento::UNKNOWN;
        e5.descripcion = "Evento desconocido de prueba";
        e5.origenModulo = "TEST";
        queueEntrada.push(e5);
    }

    tAnalisis.join();
    //------------------------------------------------------------------------------------------------------------------------------------------------
    iniciarJSON();
    

    return 0;
}