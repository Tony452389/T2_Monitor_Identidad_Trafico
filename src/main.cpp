#include <iostream>
#include <thread>

#include "Evento.h"
#include "EventQueue.h"

#include "Analisis.h"
#include "Identidad.h"
#include "Sniffer.h"
#include "JSONGen.h"

//Colas globales del sistema
EventQueue queueEntrada;
EventQueue queueSalida;

int main(){
    std::cout << "==========================================" << std::endl;
    std::cout << " Monitor de Identidad y Trafico de Red" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "\nEl sistema se ejecuta en modo monitoreo continuo." << std::endl;
    std::cout << "Presione CTRL + C para detener la ejecucion." << std::endl;
    std::cout << "Nota: el modulo Sniffer puede requerir permisos (sudo)." << std::endl;
    std::cout << std::endl;

    std::cout << "Iniciando sistema en 5 segundos..." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));

    //Iniciar modulos como hilos
    std::thread hiloSniffer(iniciarSniffer);
    std::thread hiloIdentidad(iniciarIdentidad);
    std::thread hiloAnalisis(iniciarAnalisis);
    std::thread hiloJSON(iniciarJSON);

    //Esperar a que los modulos terminen
    hiloSniffer.join();
    hiloIdentidad.join();
    hiloAnalisis.join();
    hiloJSON.join();

    return 0;
}