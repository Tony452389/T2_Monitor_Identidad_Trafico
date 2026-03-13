###         Monitor híbrido de identidad digital y tráfico anómalo (C++)      ###

## Descripción
Herramienta en C++ para Linux que monitorea cambios en identidad digital (IP/MAC), captura tráfico de red, detecta patrones anómalos y genera reportes estructurados en JSON.
El sistema se compone de varios módulos independientes que se comunican mediante una cola de eventos (EventQueue).


## Descripción de módulos
Identidad
Monitorea cambios de IP y MAC de la interfaz seleccionada.

Sniffer
Captura tráfico de red usando libpcap.

Analisis
Clasifica eventos y detecta anomalías.

JSONGen
Genera el reporte final en formato JSON.

## Estructura del proyecto
src/
    main.cpp
    Identidad.cpp
    Sniffer.cpp
    Analisis.cpp
    JSONGen.cpp
    Network.cpp

include/
    Identidad.h
    Sniffer.h
    Analisis.h
    JSONGen.h
    Evento.h
    EventQueue.h
    Network.h

json/
    .gitkeep

README.md
Makefile
.gitignore


## Requisitos
Para compilar el proyecto se necesita:
g++
libpcap
pthread

Linux o entorno compatible
Ubuntu / Debian
sudo apt update
sudo apt install g++ libpcap-dev


## Compilación
Desde la raíz del proyecto:
g++ src/*.cpp -Iinclude -o monitor -lpcap -pthread


## Ejecución
sudo ./monitor

Dependiendo del sistema puede requerirse:
sudo ./monitor

para permitir captura de paquetes de red.

## Flujo de trabajo con Git
No se debe trabajar directamente en main.

## Reglas del proyecto
No modificar main.cpp sin consenso del equipo.
No modificar Evento.h sin discutirlo primero.
No modificar EventQueue.h sin consenso.
Cada integrante trabaja únicamente en su módulo.
