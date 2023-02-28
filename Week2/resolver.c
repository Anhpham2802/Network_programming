#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int check(char *test)
{
    for (int i = 0; i < strlen(test); i++)
    {
        if (test[i] < 48 || test[i] > 57)
        {
            return 0;//la hostname
        }
    }
    return 1; // la ip
}

int getHostName(char *hostname, char *ip)
{
	struct in_addr addr;
	struct hostent* host;
    inet_aton(ip, &addr);
    
    host = gethostbyaddr(((const char*)&addr),sizeof(addr), AF_INET);
    if(host == NULL)
    {
    	printf("Khong tim thay thong tin\n");
    	hostname = NULL;
    	return 1;
    }
    strcpy(hostname, host->h_name);
    return 0;
}

int get_ip(char * hostname , char* ip) 
{
	struct hostent *host;     
    struct in_addr **addr;     
    int i;     
    if ((host = gethostbyname(hostname)) == NULL)     
    {        
    	printf("Khong tim thay thong tin\n");
    	ip = NULL;
    	return 1;
    }     
    addr = (struct in_addr **) host->h_addr_list;
    for(i = 0; addr[i] != NULL; i++)
    {
    	strcpy(ip , inet_ntoa(*addr[i]) );
    }
    return 0;
}

int main(int argc , char *argv[])
{
	if(argc <2)     
    {
    	printf("Vui long nhap dia chi ip hoac ten mien");         
    	exit(1);
    }
    char *test = argv[1];
     if(check(test) == 0)
    {
        char *hostname = argv[1];
        char ip[100];
        if(!get_ip(hostname , ip))
        {
            printf("Official IP is: %s\n", ip);
        }
    }
    else
    {
        char *ip = argv[1];
        char hostname[100];
        if(!getHostName(hostname, ip))
        {
            printf("Official name is: %s\n", hostname);
        }
    }
} 