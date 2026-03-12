#include "Sniffer.h"
#include <iostream>
#include <pcap.h>

#include "Evento.h"
#include "EventQueue.h"

extern EventQueue queueEntrada;

void procesarPaquete(u_char* args,
                     const struct pcap_pkthdr* header,
                     const u_char* packet)
{
    int N = 24;

    std::cout << "\nPaquete capturado\n";
    std::cout << "Primeros " << N << " bytes: ";

    for (int i = 0; i < N && i < header->len; i++)
    {
        printf("%02x ", packet[i]);
    }

    std::cout << std::endl;

    // Crear evento para el sistema
    Evento e;

    e.tipo = TipoEvento::UNKNOWN;
    e.origenModulo = "Sniffer";
    e.descripcion = "Paquete capturado por el sniffer";

    // enviar evento a la cola del sistema
    queueEntrada.push(e);
}

void iniciarSniffer()
{
    std::cout << "Modulo Sniffer activo" << std::endl;

    char errbuf[PCAP_ERRBUF_SIZE];
    
    //  Interfaz y IP Pruebas
    const char* interfaz = "enp0s3";
    const char* ip_local = "10.0.2.15";

    pcap_t* handle = pcap_open_live(
        interfaz,
        BUFSIZ,
        1,
        1000,
        errbuf
    );

    if (handle == nullptr)
    {
        std::cerr << "Error al abrir interfaz: " << errbuf << std::endl;
        return;
    }

    std::cout << "Sniffing en interfaz: " << interfaz << std::endl;

    struct bpf_program filtro;

    std::string expresion = "host ";
    expresion += ip_local;

    if (pcap_compile(handle, &filtro, expresion.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        std::cerr << "Error compilando filtro BPF" << std::endl;
        return;
    }

    if (pcap_setfilter(handle, &filtro) == -1)
    {
        std::cerr << "Error aplicando filtro BPF" << std::endl;
        return;
    }

    std::cout << "Filtro aplicado: " << expresion << std::endl;

    pcap_loop(handle, 0, procesarPaquete, nullptr);

    pcap_close(handle);
}
