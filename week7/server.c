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
#define BACKLOG 10

struct Account
{
    char username[20];
    char password[20];
};
// account node
typedef struct node
{
    char name[MAX];
    char pass[MAX];
    int status;
    struct node *next;
} node_t;

// danh sách liên kết
typedef struct node_l
{
    char user_name[MAX];
    int sockfd;
    struct node_l *next;
} node_l;

node_t *list_acc = NULL;
node_l *list_client = NULL;
char chat_history[8192];
int byte_recei, byte_sent;

// load dữ liệu từ file text vào danh sách liên kết
node_t *upload_file(char *filename)
{
    int status; // kiểm tra có bao nhiêu tài khoản
    FILE *f;
    char name[MAX], pass[MAX];
    node_t *head, *current;
    head = current = NULL;

    // open file data
    printf("\nĐang tải dữ liệu!\n");

    if ((f = fopen(filename, "r")) == NULL)
    {
        printf("Không thể mở file!\n");
        exit(0);
    }

    // cập nhật tên, mật khẩu, trạng thái vào danh sách liên kết
    while (fscanf(f, "%s %s %d\n", name, pass, &status) != EOF)
    {
        // create new node
        node_t *node = malloc(sizeof(node_t));
        strcpy(node->name, name);
        strcpy(node->pass, pass);
        node->status = status;

        // add node to list
        if (head == NULL)
            current = head = node;
        else
            current = current->next = node;
    }

    fclose(f);
    printf("Hoàn tất tải dữ liệu!\n");
    return head;
}

// tìm tài khoản trong danh sách liên kết
node_t *search(node_t *head, char *name)
{
    node_t *current = head;
    while (current != NULL)
    {
        if (0 == strcmp(current->name, name))
            return current;
        current = current->next;
    }
    return NULL;
}
// load lịch sử chat
void load_history()
{
    FILE *f;
    char line[MAX];
    if ((f = fopen("groupchat.txt", "r")) == NULL)
    {
        printf("Khong the mo file!\n");
        exit(0);
    }
    while (fgets(line, MAX, f) != NULL)
    {
        strcat(chat_history, line);
    }
    fclose(f);
}

// lưu tin nhắn vào file
void save_history(char *msg)
{
    FILE *f;
    if ((f = fopen("groupchat.txt", "a")) == NULL)
    {
        printf("Khong the mo file!\n");
        exit(0);
    }
    fprintf(f, "\n%s", msg);
    fclose(f);
}
// thêm tài khoản vào danh sách liên kết
node_l *add_client(node_l *head, char *name, int sockfd)
{
    node_l *node = malloc(sizeof(node_l));
    strcpy(node->user_name, name);
    node->sockfd = sockfd;
    node->next = NULL;
    if (head == NULL)
        head = node;
    else
    {
        node->next = head;
        head = node;
    }
    return head;
}
// xóa tài khoản trong danh sách liên kết
node_l *remove_client(node_l *head, char *name, int sockfd)
{
    node_l *current = head;
    node_l *prev = NULL;
    while (current != NULL)
    {
        if (current->sockfd == sockfd)
        {
            if (prev == NULL)
                head = current->next;
            else
                prev->next = current->next;
            free(current);
            return head;
        }
        prev = current;
        current = current->next;
    }
    return head;
}

// tìm tài khoản trong danh sách liên kết
node_l *search_client(node_l *head, char *name, int sockfd)
{
    node_l *current = head;
    while (current != NULL)
    {
        if (current->sockfd == sockfd)
            return current;
        current = current->next;
    }
    return NULL;
}
// gửi tin nhắn đến client
void *client_handler(void *fd)
{
    int conn_sock = *((int *)fd);
    char buff[MAX];
    char user[MAX];
    char msg[MAX];

    // Duyet danh sach client lay ten nguoi dung
    node_l *current = list_client;
    while (current != NULL)
    {
        if (current->sockfd == conn_sock)
        {
            sprintf(user, "%s", current->user_name);
            break;
        }
        current = current->next;
    }
    // gui lich su chat
    if (0 >= (byte_sent = send(conn_sock, chat_history, strlen(chat_history), 0)))
    {
        printf("\nLỗi \n");
        exit(0);
    }
    // nhan và gui tin nhan
    while ((byte_recei = recv(conn_sock, buff, MAX - 1, 0)) > 0)
    {
        buff[byte_recei] = '\0';
        sprintf(msg, "%s: %s", user, buff);
        strcat(chat_history, "\n");
        strcat(chat_history, msg);
        save_history(msg);

        // gui tin nhan den tat ca client
        current = list_client;
        while (current != NULL)
        {
            if (current->sockfd != conn_sock)
            {
                if (0 > (byte_sent = send(current->sockfd, msg, strlen(msg), 0)))
                {
                    printf("\nError: \n");
                    exit(0);
                }
            }
            else
            {
                if (0 > (byte_sent = send(conn_sock, "", strlen(""), 0)))
                {
                    printf("\nError: \n");
                    exit(0);
                }
            }
            current = current->next;
        }
    }
}

int main()
{
    int listen_sock, conn_sock;
    char buff[MAX];
    struct sockaddr_in server_addr;
    struct Account acc;
    int sin_size;
    struct sockaddr_in client;
    pthread_t tid;

    // Xây dựng TCP socket
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("\nLỗi ");
        return 0;
    }

    // bind server address
    memset(&server_addr, '\0', sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi("5500"));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("\nLỗi ");
        return 0;
    }
    // nhận phản hồi từ client
    if (listen(listen_sock, BACKLOG) == -1)
    {
        perror("\nLỗi ");
        return 0;
    }

    list_acc = upload_file("taikhoan.txt");
    // load lịch sử chat
    load_history();

    while (1)
    {
        // chấp nhận kết nối từ client
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
            perror("\nLỗi ");
        printf("\nNew connection from %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        // nhận thông tin đăng nhập từ client
        if (0 >= (byte_recei = recv(conn_sock, &acc, sizeof(struct Account), 0)))
        {
            printf("\nLỗi \n");
            exit(0);
        }
        node_t *acc_node = search(list_acc, acc.username);
        // check tên đăng nhập
        if (acc_node == NULL)
        {
            if (0 >= (byte_sent = send(conn_sock, "Dang nhap khong thanh cong!", strlen("Dang nhap khong thanh cong!"), 0)))
            {
                printf("\nLỗi \n");
                break;
            }
        }
        else
        { // check password
            if (strcmp(acc.password, acc_node->pass) != 0)
            {
                if (0 >= (byte_sent = send(conn_sock, "Dang nhap khong thanh cong!", strlen("Dang nhap khong thanh cong!"), 0)))
                {
                    printf("\nLỗi \n");
                    break;
                }
            }
            // check trạng thái
            if (acc_node->status == 0)
            {
                if (0 >= (byte_sent = send(conn_sock, "Dang nhap khong thanh cong!", strlen("Dang nhap khong thanh cong!"), 0)))
                {
                    printf("\nLỗi \n");
                    break;
                }
            }
        }
        // gửi phản hồi đến client
        if (0 >= (byte_sent = send(conn_sock, "Dang nhap thanh cong!", strlen("Dang nhap thanh cong!"), 0)))
        {
            printf("\nLỗi \n");
            exit(0);
        }

        // thêm client vào danh sách
        list_client = add_client(list_client, acc.username, conn_sock);

        // tạo thread cho mỗi client
        pthread_create(&tid, NULL, &client_handler, (void *)&conn_sock);

    }
    close(conn_sock);
    close(listen_sock);
    return 0;
}