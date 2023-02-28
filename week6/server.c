#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 5
#define MAX 1024

// account node
typedef struct node
{
	char name[MAX];
	char pass[MAX];
	int status;
	struct node *next;
} node_t;

// load dữ liệu từ file text vào danh sách liên kết
node_t *upload_file(char *filename)
{
	int status; // kiểm tra có bao nhiêu tài khoản
	FILE *f;
	char name[MAX], pass[MAX];
	node_t *head, *current;
	head = current = NULL;

	// open file data
	printf("Đang tải dữ liệu!\n");

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

// cập nhật trạng thái tài khoản
void saveList(node_t *head, char *filename)
{
	FILE *f;
	f = fopen(filename, "w");
	node_t *current;
	for (current = head; current; current = current->next)
		fprintf(f, "%s %s %d\n", current->name, current->pass, current->status);
	fclose(f);
}

int main(int argc, char const *argv[])
{
	char filename[] = "account.txt";

	// kiểm tra đầu vào
	if (argc != 2)
	{
		printf("Nhập theo cú pháp: ./server PortNumber\n\n");
		return 0;
	}

	int listen_sock, conn_sock;
	char name[MAX], pass[MAX], *reply;
	int b_sent, b_recei;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t sin_size;
	node_t *found;
	int pid;

	// load data
	node_t *account_list = upload_file(filename);

	// Xây dựng TCP socket
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("\nLỗi!");
		return 0;
	}

	// Liên kết địa chỉ với socket
	memset(&server, '\0', sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("\nLỗi!");
		return 0;
	}

	// nhận phản hồi từ client
	if (listen(listen_sock, BACKLOG) == -1)
	{
		perror("\nLỗi!");
		return 0;
	}

	puts("Máy chủ đang tải\n");

	// Giao tiếp với client
	while (1)
	{
		// chấp nhận kết nối từ client
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
			perror("\nError: ");
		// in ra địa chỉ IP của client đã kết nối
		printf("\nBạn đã kết nối từ %s\n", inet_ntoa(client.sin_addr));

		// bắt đầu giao tiếp bằng fork
		pid = fork();
		if (pid < 0)
		{
			perror("Lỗi!");
			return 1;
		}

		// tiến trình con
		if (pid == 0)
		{
			// nhận tên tài khoản từ client
			if (0 >= (b_recei = recv(conn_sock, name, MAX - 1, 0)))
			{
				printf("\nMất kết nối!!\n");
				break;
			}
			name[b_recei] = '\0';

			// kiểm tra tài khoản có tồn tại không
			if ((found = search(account_list, name)))
			{
				if (found->status == 1)
					reply = "1"; // tìm kiếm tên tài khoản thành công
				else
					reply = "2"; // tìm thấy nhưng tài khoản đã bị khóa
			}
			else
				reply = "0"; // không tìm thấy tên tài khoản

			// gửi phản hồi
			if (0 >= (b_sent = send(conn_sock, reply, strlen(reply), 0)))
			{
				printf("\nMất kết nối!");
				break;
			}

			int count = 0; // đếm số lần nhập sai mật khẩu

			while (1)
			{
				// nhận mật khẩu từ client
				memset(pass, '\0', MAX);
				if (0 >= (b_recei = recv(conn_sock, pass, MAX - 1, 0)))
				{
					printf("\nConnection closed\n");
					break;
				}
				pass[b_recei] = '\0';

				// kiểm tra mật khẩu
				if (0 == strcmp(found->pass, pass))
					reply = "1"; // mật khẩu đúng
				else
				{
					count++;
					if (count == 3)
					{
						reply = "2";	   // sai quá 3 lần
						found->status = 0; // khóa tài khoản
					}
					else
						reply = "0"; // sai mật khẩu nhưng chưa đủ 3 lần
				}

				// gửi phản hồi
				if (0 >= (b_sent = send(conn_sock, reply, strlen(reply), 0)))
				{
					printf("\nConnection closed\n");
					break;
				}
			}
			// lưu lại trạng thái tài khoản
			saveList(account_list, filename);
			close(conn_sock);
		}
		else
		{
			// đóng socket của tiến trình con
			close(conn_sock);
		}
	}
	close(listen_sock);
	return 0;
}