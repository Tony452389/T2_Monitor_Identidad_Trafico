#include <iostream>

#include "Analisis.h"
#include "Identidad.h"
#include "Sniffer.h"
#include "JSONGen.h"

int main()
{
    std::cout << "Sistema de monitoreo iniciado" << std::endl;

    iniciarIdentidad();
    iniciarSniffer();
    iniciarAnalisis();
    iniciarJSON();

    return 0;
}