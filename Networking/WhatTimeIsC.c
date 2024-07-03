#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <time.h>

struct ntp_packet
{
    unsigned char leap_version_mode;
    unsigned char stratum;
    unsigned char poll;
    unsigned char precision;
    unsigned int root_delay;
    unsigned int root_dispertion;
    char refID[4];
    unsigned long long ref_timestamp;
    unsigned long long original_timestamp;
    unsigned long long receive_timestamp;
    unsigned long long transmit_timestamp;

};

int main(int argc, char **argv)
{
    struct ntp_packet my_packet;
#ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data;
    if (WSAStartup(0x0202, &wsa_data))
    {
        printf("unable to initialize winsock2 \n");
        return -1;
    }
#endif

     time_t now;
     time(&now);

     my_packet.leap_version_mode = 0 << 6 | 4 << 3 | 3;
     my_packet.stratum = 0;
     my_packet.poll = 0;
     my_packet.precision= 0;
     my_packet.root_delay = 0;
     my_packet.root_dispertion = 0;
     (memset(my_packet.refID,0,sizeof(my_packet.refID)));
     my_packet.ref_timestamp = 0;
     my_packet.original_timestamp = 0;
     my_packet.receive_timestamp = 0;
     my_packet.transmit_timestamp = 0;


     struct sockaddr_in sin;
     sin.sin_family = AF_INET;
     sin.sin_port = htons(123);

      int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0)
    {
        printf("unable to initialize the UDP socket \n");
        return -1;
    }

    printf("Seconds from 1970: %lld\n",(long long)(now));
    printf("socket %d created \n", s);

     inet_pton(AF_INET, "216.239.35.12", &sin.sin_addr); // this will create a big endian 32 bit address
     int sent_bytes = sendto(s, &my_packet, sizeof(my_packet), 0, (struct sockaddr *)&sin, sizeof(sin));
    //  printf("sent %d bytes via UDP \n", sent_bytes);

    recv(s,&my_packet,sizeof(struct ntp_packet),0);

    return 0;
}