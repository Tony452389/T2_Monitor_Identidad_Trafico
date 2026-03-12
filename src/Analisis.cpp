#include <iostream>
#include <set>

#include "Analisis.h"
#include "Evento.h"
#include "EventQueue.h"

//Comando para compilar: g++ src/*.cpp -Iinclude -o monitor -lpcap -pthread
//             ejecutar: ./monitor 

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
        case TipoEvento::ARP:
            contadorARP++;
            
            break;

        case TipoEvento::ICMP:
            contadorICMP++;
            
            break;
        
        case TipoEvento::IP_CHANGE:
            contadorIP++;
            ultimaIP = evento.ipOrigen;

            break;

        case TipoEvento::MAC_CHANGE:
            contadorMAC++;
            ultimaMAC = evento.macOrigen;

            break;

        case TipoEvento::UNKNOWN:
            contadorUNKNOWN++;
            
            break;

        default:
            break;
    }

    //Aqui se actualizaran variables contadoras y registros de informacion

    //registrar IP si existe
    if(!evento.ipOrigen.empty()){
        ipsConocidas.insert(evento.ipOrigen);
        
    }
}

/*
-----------------------------------------------------
            Evaluar Reglas de Anomalia
-----------------------------------------------------
*/
void detectarAnomalias(const Evento& evento){

    //Aqui se implementaran las reglas definidas

    /*
    if(contadorICMP > 5){
        generarAnomalia(
            "ICMP repetitivo detectado",
            NivelRiesgo::MEDIO,
            evento
        );
    }
    */
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

    std::cout << "[Analisis]: ANOMALIA" << descripcion << "\n";
}


