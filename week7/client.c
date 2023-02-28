#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

#define MAX 1024

struct Account
{
    char username[20];
    char password[20];
};
struct Account acc;
// nhận tin nhắn từ server
void *receive(void *arg)
{
    int listen_sock = *((int *)arg);
    char buff[MAX];
    int byte_recei;
    while (1)
    {
        if (0 > (byte_recei = recv(listen_sock, buff, MAX - 1, 0)))
        {
            printf("\nLỗi!! Không nhận được phản hồi từ máy chủ!\n");
            exit(0);
        }
        buff[byte_recei] = '\0';
        if (strlen(buff) > 0)
        {
            printf("\n%s", buff);
        }
        printf("\n[%s]: ", acc.username);
        fflush(stdout);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        printf("Nhập theo cú pháp: ./client IPAddress PortNumber username password\n\n");
    }

    int listen_sock;
    int byte_recei, byte_sent, msg_size;
    char buff[MAX], text[MAX];
    struct sockaddr_in server_addr;

    // xay dung socket
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    // bind server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // Kiểm tra kết nối đến server
    if (connect(listen_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        printf("\nLỗi!! không thể kết nối đến máy chủ!");
        return 0;
    }

    // gửi username và password

    strcpy(acc.username, argv[3]);
    strcpy(acc.password, argv[4]);
    send(listen_sock, &acc, sizeof(struct Account), 0);

    // nhận phản hồi từ server
    if (0 >= (byte_recei = recv(listen_sock, buff, MAX - 1, 0)))
    {
        printf("\nLỗi!! Không nhận được phản hồi từ máy chủ!\n");
        exit(0);
    }
    buff[byte_recei] = '\0';

    if (strcmp(buff, "Dang nhap thanh cong!") != 0)
    {
        return 1;
    }

    if (0 >= (byte_recei = recv(listen_sock, buff, MAX - 1, 0)))
    {
        printf("\nLỗi!! Không nhận được phản hồi từ máy chủ!\n");
        exit(0);
    }
    buff[byte_recei] = '\0';
    printf("%s\n", buff);

    // tạo luồng nhận tin nhắn
    pthread_t tid;
    pthread_create(&tid, NULL, receive, &listen_sock);
    while (1)
    {
        // nhận tin nhắn từ bàn phím
        printf("[%s]: ", acc.username);
        fgets(text, MAX, stdin);
        text[strlen(text) - 1] = '\0';
        if (strcmp(text, "") == 0)
        {
            break;
        }
        // gửi tin nhắn đến server
        send(listen_sock, text, strlen(text), 0);
        }
    close(listen_sock);
    return 0;
}