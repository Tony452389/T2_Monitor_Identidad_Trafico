#include "Sniffer.h"
#include <iostream>
#include <pcap.h>

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
}

void iniciarSniffer()
{
    std::cout << "Modulo Sniffer activo" << std::endl;

    char errbuf[PCAP_ERRBUF_SIZE];

    // PONER INTERFAZ
    const char* interfaz = "enp0s3";

    // PONER IP
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
