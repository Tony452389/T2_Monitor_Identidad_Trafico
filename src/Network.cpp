#include "Network.h"
#include <stdexcept>
#include <pcap.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Interface getActiveInterface()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;

    if (pcap_findalldevs(&alldevs, errbuf) == -1)
        throw std::runtime_error(std::string("pcap_findalldevs: ") + errbuf);

    Interface result;

    for (pcap_if_t* dev = alldevs; dev != nullptr; dev = dev->next)
    {
        bool isLoopback = (dev->flags & PCAP_IF_LOOPBACK) != 0;
        bool isUp       = (dev->flags & PCAP_IF_UP)       != 0;

        if (isLoopback || !isUp)
            continue;

        for (pcap_addr_t* addr = dev->addresses; addr != nullptr; addr = addr->next)
        {
            if (addr->addr && addr->addr->sa_family == AF_INET)
            {
                char ipbuf[INET_ADDRSTRLEN];
                auto* sin = reinterpret_cast<struct sockaddr_in*>(addr->addr);
                inet_ntop(AF_INET, &sin->sin_addr, ipbuf, sizeof(ipbuf));

                result.name = dev->name;
                result.ipv4 = ipbuf;
                break;
            }
        }

        if (!result.name.empty())
            break;
    }

    pcap_freealldevs(alldevs);

    if (result.name.empty())
        throw std::runtime_error("No active non-loopback interface with IPv4 found");

    return result;
}
