# T2 - Monitor de Identidad y Tráfico de Red

Herramienta desarrollada en C++ para Linux que permite capturar tráfico de red, detectar eventos relevantes en la red local y analizar posibles anomalías relacionadas con cambios de identidad digital (IP/MAC) o comportamiento de red.

El sistema utiliza captura de paquetes mediante **libpcap**, procesamiento concurrente mediante **threads**, y comunicación entre módulos mediante una **cola de eventos thread-safe**.

---
# Autoría
Integrantes:
-Marco Antonio Guadalupe Vargas Ruiz
-Sergio Pedro Sepulveda Rodriguez
-Josue Israel Castro Aguilar
-Ricardo Hervey Estrada Garcia

Proyecto desarrollado como parte de la actividad académica:

T2 - Monitor de Identidad y Tráfico de Red

---

# Arquitectura del sistema

El sistema está diseñado de forma modular, donde cada componente tiene una responsabilidad específica dentro del flujo de monitoreo.

Flujo general de eventos:

Sniffer → queueEntrada → Analisis → queueSalida → JSONGen

Cada módulo se ejecuta de manera concurrente mediante **hilos (threads)**.

---

# Módulos del sistema

## Sniffer
Responsable de capturar paquetes de red utilizando **libpcap**.

Este módulo:

- captura tráfico en la interfaz de red activa
- identifica tipos básicos de tráfico
- extrae información relevante del paquete

Información extraída:

- MAC origen
- MAC destino
- IP origen
- IP destino
- timestamp

Clasificación inicial de eventos:

- ARP
- ICMP
- IP
- UNKNOWN

Los eventos generados se envían a la cola compartida:

queueEntrada

---

## Analisis
Consume eventos desde `queueEntrada` y aplica reglas para detectar posibles anomalías en el comportamiento de la red.

Ejemplos de reglas implementadas:

- ARP repetitivo
- ICMP repetitivo
- IP desconocida
- cambios frecuentes de IP
- cambios frecuentes de MAC
- tráfico desconocido masivo

Cuando una regla se activa, el módulo genera un nuevo evento de tipo:

ANOMALY

que es enviado a:

queueSalida

---

## Network
Módulo auxiliar encargado de detectar automáticamente la interfaz de red activa y su dirección IPv4.

Esto permite que el Sniffer funcione sin depender de interfaces estáticas como:

eth0  
enp0s3

---

## Identidad
Este módulo fue diseñado para monitorear cambios en la identidad de red de una interfaz.

Funcionalidad prevista:

- detectar cambios en la dirección MAC
- detectar cambios en la dirección IP
- generar eventos de cambio de identidad
- enviar dichos eventos al flujo principal del sistema

Durante el desarrollo se implementó una versión funcional del monitoreo de identidad, sin embargo la integración final con la estructura de eventos del sistema quedó pendiente.

Por esta razón el módulo **se encuentra presente en el repositorio pero temporalmente deshabilitado en `main.cpp`** para evitar conflictos de compilación.

---

## JSONGen
Este módulo fue diseñado para generar reportes estructurados en formato **JSON** a partir de los eventos detectados por el sistema.

Funcionalidad prevista:

- consumir eventos desde `queueSalida`
- almacenarlos en una estructura JSON
- guardar reportes en archivo

La implementación base del módulo se encuentra incluida en el proyecto, sin embargo su integración completa al flujo del sistema no se finalizó durante el desarrollo.

Por esta razón el módulo **se mantiene presente en el repositorio pero deshabilitado temporalmente en `main.cpp`**.

---

# Comunicación entre módulos

La comunicación entre módulos se realiza mediante una **cola de eventos thread-safe** implementada en:

EventQueue

Esta estructura utiliza:

- `std::mutex`
- `std::condition_variable`
- `std::queue`

para permitir el intercambio seguro de eventos entre múltiples hilos.

Existen dos colas principales:

queueEntrada  
Eventos generados por Sniffer o Identidad.

queueSalida  
Eventos generados por el módulo Analisis.

---

# Ejecución del programa

El sistema funciona como un monitor continuo de tráfico de red.

El programa permanece ejecutándose hasta que el usuario lo detenga manualmente.

Para finalizar la ejecución:

CTRL + C

---

# Requisitos

Sistema operativo:

Linux

Dependencias necesarias:

- g++
- libpcap
- pthread

---

# Compilación

Ejemplo de compilación:

g++ src/*.cpp -o monitor -lpcap -pthread

---

# Ejecución

La captura de paquetes requiere permisos elevados, por lo que se recomienda ejecutar el programa con:

sudo ./monitor

---

# Estructura del proyecto

include/
 Analisis.h
 EventQueue.h
 Evento.h
 Identidad.h
 JSONGen.h
 Network.h
 Sniffer.h

src/
 Analisis.cpp
 Identidad.cpp
 JSONGen.cpp
 Network.cpp
 Sniffer.cpp
 main.cpp
json/
 .gitkeep
---

# Estado del proyecto

Módulos completamente funcionales:

- Sniffer
- Analisis
- Network

Módulos presentes en el repositorio:

- Identidad
- JSONGen

Estos módulos forman parte del diseño original del sistema, pero actualmente se encuentran **deshabilitados temporalmente en `main.cpp`** para mantener la estabilidad del sistema principal.


