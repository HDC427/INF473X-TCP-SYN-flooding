#ifndef UTILITY_H_
#define UTILITY_H_

#define TEST_STRING "syn flood";

struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t length;
};

char *rangeIP(char IpAddr[],char buf[]); // function for generation random ip adress

unsigned short checksum(unsigned short *ptr, int nbytes);

void syn(char *source_ip, int source_port, char *dest_ip, int dest_port);

#endif