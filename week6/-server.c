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

int s_sock, connect_sock;
char name[MAX], pass[MAX], *reply;
int b_sent, b_recei;
struct sockaddr_in server;
struct sockaddr_in client;
socklen_t sin_size;
node_t *found;
char file[] = "account.txt";

void saveFile(node_t *head, char *file);
node_t *uploadFile(char *file);
node_t *search(node_t *head, char *name);

int main(int argc, char const *argv[])
{
	// kiểm tra đầu vào
	if (argc != 2)
	{
		printf("Nhập theo cú pháp: ./server PortNumber\n\n");
		return 0;
	}

	// Xây dựng TCP socket
	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("\nLỗi!");
		return 0;
	}

	// Liên kết địa chỉ với socket
	memset(&server, '\0', sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("\nLỗi!");
		return 0;
	}

	// nhận phản hồi từ client
	if (listen(s_sock, BACKLOG) == -1)
	{
		perror("\nLỗi! Không nhận được phản hồi!");
		return 0;
	}

	puts("Máy chủ đang tải...");
	// load data
	node_t *account = uploadFile(file);

	// Giao tiếp với client
	while (1)
	{
		// chấp nhận kết nối từ client
		sin_size = sizeof(struct sockaddr_in);
		if ((connect_sock = accept(s_sock, (struct sockaddr *)&client, &sin_size)) == -1)
			perror("\nLỗi! Không thể kết nối với người dùng!");
		// in ra địa chỉ IP của client đã kết nối
		printf("\nKết nối đến từ %s\n", inet_ntoa(client.sin_addr));

		int rep = 0;
		do
		{
			// bắt đầu giao tiếp bằng fork
			rep = test_login(account);
		} while (rep == 0);
		close(connect_sock);
	}
	close(s_sock);
	return 0;
}

void test_login(node_t *account)
{
	int pid = fork();
	if (pid < 0)
	{
		perror("Lỗi!");
		return 1;
	}

	if (pid == 0)
	{
		// nhận tên tài khoản từ client
		if (0 >= (b_recei = recv(connect_sock, name, MAX - 1, 0)))
		{
			printf("\nĐóng kết nối!!\n");
			return 0;
		}
		name[b_recei] = '\0';

		// kiểm tra tài khoản có tồn tại không
		if ((found = search(account, name)))
		{
			if (found->status == 1)
				reply = "1"; // tìm kiếm tên tài khoản thành công
			else
				reply = "2"; // tìm thấy nhưng tài khoản đã bị khóa
		}
		else
			reply = "0"; // không tìm thấy tên tài khoản

		// gửi phản hồi
		if (0 >= (b_sent = send(connect_sock, reply, strlen(reply), 0)))
		{
			printf("\nĐóng kết nối!!");
			return 0;
		}

		int count = 0; // đếm số lần nhập sai mật khẩu

		while (1)
		{
			// nhận mật khẩu từ client
			memset(pass, '\0', MAX);
			if (0 >= (b_recei = recv(connect_sock, pass, MAX - 1, 0)))
			{
				printf("\nĐóng kết nối!!\n");
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
									   // lưu lại trạng thái tài khoản
					saveFile(account, file);
				}
				else
					reply = "0"; // sai mật khẩu nhưng chưa đủ 3 lần
			}
			// gửi phản hồi
			if (0 >= (b_sent = send(connect_sock, reply, strlen(reply), 0)))
			{
				printf("\nĐóng kết nối!!\n");
				return 0;
			}
			close(connect_sock);
		}
	}
}
// load dữ liệu từ file text vào danh sách liên kết
node_t *uploadFile(char *file)
{
	int status; // kiểm tra có bao nhiêu tài khoản
	FILE *f;
	char name[MAX], pass[MAX];
	node_t *head, *current;
	head = current = NULL;

	// open file
	if ((f = fopen(file, "r")) == NULL)
	{
		printf("Không thể mở file!\n");
		exit(0);
	}

	printf("Đang tải dữ liệu...\n");
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
	printf("Hoàn tất!!\n\n");
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
void saveFile(node_t *head, char *file)
{
	FILE *f;
	f = fopen(file, "w");
	node_t *current;
	for (current = head; current; current = current->next)
		fprintf(f, "%s %s %d\n", current->name, current->pass, current->status);
	fclose(f);
}