/*
-----------------------------------------------------------------------------------
Modulo: Sniffer
Encargado del modulo: Marco Antonio Guadalupe Vargas Ruiz
Proyecto: T2 - Monitor de Identidad y Trafico
-----------------------------------------------------------------------------------
*/

#include "Sniffer.h"
#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Evento.h"
#include "EventQueue.h"
#include "Network.h"

extern EventQueue queueEntrada;

std::string obtenerTimestamp()
{
    auto t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

std::string macToString(const u_char* mac)
{
    std::ostringstream oss;

    oss << std::hex << std::setfill('0')
        << std::setw(2) << (int)mac[0] << ":"
        << std::setw(2) << (int)mac[1] << ":"
        << std::setw(2) << (int)mac[2] << ":"
        << std::setw(2) << (int)mac[3] << ":"
        << std::setw(2) << (int)mac[4] << ":"
        << std::setw(2) << (int)mac[5];

    return oss.str();
}

void procesarPaquete(
    u_char* args,
    const struct pcap_pkthdr* header,
    const u_char* packet)
{
    const int N = 24;

    std::cout << "\nPaquete capturado\n";
    std::cout << "Primeros " << N << " bytes: ";

    for (int i = 0; i < N && i < header->len; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)packet[i] << " ";
    }

    std::cout << std::dec << std::endl;

    // Validar tamaño mínimo para Ethernet
    if (header->len < sizeof(struct ether_header))
    {
        return;
    }

    struct ether_header* eth =
        (struct ether_header*)packet;

    Evento e;

    e.origenModulo = "Sniffer";
    e.descripcion = "Paquete capturado";
    e.timestamp = obtenerTimestamp();

    e.macOrigen = macToString(eth->ether_shost);
    e.macDestino = macToString(eth->ether_dhost);

    uint16_t etherType = ntohs(eth->ether_type);

    if (etherType == ETHERTYPE_ARP)
    {
        e.tipo = TipoEvento::ARP;
        e.descripcion = "Paquete ARP detectado";
    }
    else if (etherType == ETHERTYPE_IP)
    {
        if (header->len < sizeof(struct ether_header) + sizeof(struct ip))
        {
            return;
        }

        struct ip* ipHeader =
            (struct ip*)(packet + sizeof(struct ether_header));

        char ipSrc[INET_ADDRSTRLEN];
        char ipDst[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &(ipHeader->ip_src), ipSrc, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), ipDst, INET_ADDRSTRLEN);

        e.ipOrigen = ipSrc;
        e.ipDestino = ipDst;

        if (ipHeader->ip_p == IPPROTO_ICMP)
        {
            e.tipo = TipoEvento::ICMP;
            e.descripcion = "Paquete ICMP detectado";
        }
        else
        {
            e.tipo = TipoEvento::UNKNOWN;
        }
    }
    else
    {
        e.tipo = TipoEvento::UNKNOWN;
    }

    queueEntrada.push(e);
}

void iniciarSniffer()
{
    std::cout << "[Sniffer]: Modulo iniciado\n" << std::endl;

    char errbuf[PCAP_ERRBUF_SIZE];

    Interface iface = getActiveInterface();

    std::string interfaz = iface.name;
    std::string ip_local = iface.ipv4;

    std::cout << "Interfaz detectada: " << interfaz << std::endl;
    std::cout << "IP detectada: " << ip_local << std::endl;

    pcap_t* handle = pcap_open_live(
        interfaz.c_str(),
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

    std::cout << "\nSniffing en interfaz: " << interfaz << std::endl;

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
