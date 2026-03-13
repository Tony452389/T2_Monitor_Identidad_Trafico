/*.....................................................................................................................................................
                                                    Autor: Josue Israel Castro Aguilar
.......................................................................................................................................................*/

#include <iostream>
#include <set>

#include "Analisis.h"
#include "Evento.h"
#include "EventQueue.h"

extern EventQueue queueEntrada;
extern EventQueue queueSalida;

//Estado interno del Modulo
int contadorICMP = 0;
int contadorARP = 0;
int contadorMAC = 0;
int contadorIP = 0;
int contadorUNKNOWN = 0;

std::set<std::string> ipsConocidas;

std::string ultimaMAC;
std::string ultimaIP;

/*
-----------------------------------------------------
            Loop Principal del Modulo
-----------------------------------------------------
*/
void iniciarAnalisis(){
    std::cout << "[Analisis]: Modulo iniciado\n";

    while(true){
        //1. Recibir evento
        Evento evento = queueEntrada.pop();

        //2. Actualizar estado interno
        actualizarEstado(evento);

        //3. Evaluar reglas de la anomalia
        detectarAnomalias(evento);
        
        //4. Reenviar evento original
        queueSalida.push(evento);
    }
}

/*
-----------------------------------------------------
            Mantener Estado Original
-----------------------------------------------------
*/
void actualizarEstado(const Evento& evento){
    switch(evento.tipo){
        case TipoEvento::ICMP:
            contadorICMP++;
            contadorARP = 0;

            break;

        case TipoEvento::ARP:
            contadorARP++;
            contadorICMP = 0;

            break;
        
        case TipoEvento::MAC_CHANGE:
            contadorMAC++;
            ultimaMAC = evento.macOrigen;

            break;

        case TipoEvento::IP_CHANGE:
            contadorIP++;
            ultimaIP = evento.ipOrigen;

            break;

        case TipoEvento::UNKNOWN:
            contadorUNKNOWN++;
            
            break;

        default:
            break;
    }

    //registrar IP si existe (Regla 8: IP nueva)
    if(!evento.ipOrigen.empty()){
        if(ipsConocidas.find(evento.ipOrigen) == ipsConocidas.end()){
            generarAnomalia(
                "IP nueva detectada en la red.",
                NivelRiesgo::BAJO,
                evento
            );

            ipsConocidas.insert(evento.ipOrigen);
        }    
    }
}

/*
-----------------------------------------------------
            Evaluar Reglas de Anomalia
-----------------------------------------------------
*/
void detectarAnomalias(const Evento& evento){
    //Regla 1: UNKNOWN Masivo
    if(contadorUNKNOWN > 5){
        generarAnomalia(
            "Trafico desconocido masivo detectado.",
            NivelRiesgo::CRITICO,
            evento
        );

        //Reiniciar contador para evitar alertas multiples
        contadorUNKNOWN = 0;
    }

    //Regla 2: Posible Spoofing de Identida (MAC_Change + IP_Change)
    else if(contadorMAC > 0 && contadorIP > 0){
        generarAnomalia(
            "Posible Spoofing de identidad detectado",
            NivelRiesgo::CRITICO,
            evento
        );

        contadorIP  = 0;
        contadorMAC = 0;
    }

    //Regla 3: MAC_Change Frecuente
    else if(contadorMAC > 2){
        generarAnomalia(
            "Cambios frecuentes de MAC detectados.",
            NivelRiesgo::ALTO,
            evento
        );

        contadorMAC = 0;
    }
    
    //Regla 4: ICMP Repetitivo
    else if(contadorICMP > 5){
        generarAnomalia(
            "ICMP repetitivo detectado.",
            NivelRiesgo::MEDIO,
            evento
        );

        contadorICMP = 0;
    }

    //Regla 5: ARP Repetitivo
    else if(contadorARP > 4){
        generarAnomalia(
            "ARP repetitivo detectado.",
            NivelRiesgo::MEDIO,
            evento
        );

        contadorARP = 0;
    }

    //Regla 6: IP_Chance Frecuente
    else if(contadorIP > 2){
        generarAnomalia(
            "Cambios frecuentes de IP detectados.",
            NivelRiesgo::MEDIO,
            evento
        );

        contadorIP = 0;
    }

    //Regla 7: UNKNOWN
    else if(evento.tipo == TipoEvento::UNKNOWN){
        generarAnomalia(
            "Evento desconocido detectado.",
            NivelRiesgo::BAJO,
            evento
        );
    }
}

/*
-----------------------------------------------------
            Crear Evento de Anomalia
-----------------------------------------------------
*/
void generarAnomalia(const std::string& descripcion, NivelRiesgo riesgo, const Evento& eventoOriginal){
    Evento anomaly;

    anomaly.tipo = TipoEvento::ANOMALY;
    anomaly.descripcion = descripcion;
    anomaly.nivelRiesgo = riesgo;
    anomaly.origenModulo = "Analisis";

    //Copiar informacion relevante
    anomaly.ipOrigen = eventoOriginal.ipOrigen;
    anomaly.macOrigen = eventoOriginal.macOrigen;
    anomaly.timestamp = eventoOriginal.timestamp;

    queueSalida.push(anomaly);

    std::cout << "\n[Analisis]: ANOMALIA -- "
              << descripcion
              << " | IP origen: " << eventoOriginal.ipOrigen
              << " | Tipo evento: " << static_cast<int>(eventoOriginal.tipo)
              << std::endl;
}


