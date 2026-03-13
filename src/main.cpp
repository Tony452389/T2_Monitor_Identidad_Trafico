/*
-----------------------------------------------------------------------------------
Modulo: Main
Encargado del modulo: Equipo del Proyecto
Proyecto: T2 - Monitor de Identidad y Trafico
-----------------------------------------------------------------------------------
*/

#include <iostream>
#include <thread>

#include "Evento.h"
#include "EventQueue.h"

#include "Analisis.h"
#include "Identidad.h"
#include "Sniffer.h"
#include "JSONGen.h"

//Activadores de modulos del sistema
#define ENABLE_SNIFFER 1
#define ENABLE_IDENTIDAD 0
#define ENABLE_ANALISIS 1
#define ENABLE_JSONGEN 0

//Colas globales del sistema
EventQueue queueEntrada;
EventQueue queueSalida;

int main(){
    std::cout << "==========================================" << std::endl;
    std::cout << " Monitor de Identidad y Trafico de Red" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "\nEl sistema se ejecuta en modo monitoreo continuo." << std::endl;
    std::cout << "Presione CTRL + C para detener la ejecucion." << std::endl;
    std::cout << "Nota: el modulo Sniffer requiere permisos (sudo)." << std::endl;
    std::cout << std::endl;

    std::cout << "Iniciando sistema en 5 segundos...\n" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));

    //Iniciar modulos como hilos
    #if ENABLE_SNIFFER
    std::thread hiloSniffer(iniciarSniffer);
    #endif
    
    #if ENABLE_IDENTIDAD
    std::thread hiloIdentidad(iniciarIdentidad);
    #endif
    
    #if ENABLE_ANALISIS
    std::thread hiloAnalisis(iniciarAnalisis);
    #endif
    
    #if ENABLE_JSONGEN
    std::thread hiloJSON(iniciarJSON);
    #endif

    //Esperar a que los modulos terminen
     #if ENABLE_SNIFFER
    hiloSniffer.join();
    #endif
    
    #if ENABLE_IDENTIDAD
    hiloIdentidad.join();
    #endif
    
    #if ENABLE_ANALISIS
    hiloAnalisis.join();
    #endif
    
    #if ENABLE_JSONGEN
    hiloJSON.join();
    #endif

    return 0;
}