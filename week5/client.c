#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int isValidIP(char *ip)
{
    int count_dots = 0, re = 0;
    for (int i = 0; ip[i] != '\0'; i++)
    {
        if (ip[i] == '.')
        {
            count_dots++;
            continue;
        }
        if (ip[i] < '0' || ip[i] > '9')
            re = 1;
    }

    if (count_dots != 3)
        re = 1;

    return re;
}

int isValidPort(char *p)
{
    for (int i = 0; p[i] != '\0'; i++)
    {
        if (p[i] < '0' || p[i] > '9')
            return 1;
    }

    port = atoi(p);

    return port > 0 && port < 65535 ? 0 : 1;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <Server IP> <Server Port>\n", argv[0]);
        return 1;
    }

    if (isValidIP(argv[1]) || isValidPort(argv[2]))
    {
        printf("Invalid IP or Port!\n");
        return 1;
    }

    int x;
    while (1)
    {
        printf("\nMENU\n"
        "--------------------------------\n"
        "1. Gửi xâu bất kỳ\n"
        "2. Gửi nội dung bất kì\n");
        printf("Chọn chức năng: ");
        scanf("%d", &x);
        switch (x)
        {
        case 1:
            printf("Chức năng 1\n");
            break;  
        case 2:
            printf("Chức năng 2\n");
            break;
        }
    }
    

    int clientfd;
    char buff[BUFF_SIZE + 1];
    struct sockaddr_in server_addr;
    // xây dựng server_addr
    lientfd = socket(AF_INET, SOCK_STREAM, 0);

    // chỉ định một địa chỉ cho ổ cắm
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // kết nối đến server
    if (connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error: Cannot connect to server!\n");
        return 1;
    }

    // giao tiếp vs server
    char buff[BUFF_SIZE];
    int ret;

    // gửi dữ liệu đến server
    printf("Send to server: ");
    gets_s(buff, BUFF_SIZE);
    ret = send(clientfd, buff, strlen(buff), 0);
    if (ret < 0)
    {
        printf("Error: Cannot send data to server!\n");
        return 1;
    }

    // nhận dữ liệu từ server
    ret = recv(clientfd, buff, BUFF_SIZE, 0);
    if (ret < 0)
    {
        printf("Error: Cannot receive data from server!\n");
        return 1;
    }
    printf("Receive from server: %s\n", buff);
    close(clientfd);
    return 0;
}