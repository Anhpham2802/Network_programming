#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 1024

int login(int c_sock, char *name, char *pass)
{
	// gửi tên tài khoản đến server
	if (0 >= (b_sent = send(c_sock, name, strlen(name), 0)))
	{
		printf("\nKết nối thất bại!\n");
		return 0;
	}

	// nhận phản hồi từ server
	if (0 >= (b_recei = recv(c_sock, buff, MAX - 1, 0)))
	{
		printf("\nLỗi!! Không nhận được phản hồi từ máy chủ!\n");
		return 0;
	}

	// Thoát nếu không tìm thấy tên tài khoản
	buff[b_recei] = '\0';
	if (0 == strcmp(buff, "2"))
	{
		puts("Tài khoản của bạn đã bị khóa!\n");
		return 0;
	}
	else if (0 == strcmp(buff, "0"))
	{
		puts("Không tìm thấy tài khoản!\n");
		return 0;
	}
}

int main(int argc, char const *argv[])
{
	// Kiểm tra cú pháp đầu vào
	if (argc != 3)
	{
		printf("Nhập theo cú pháp: ./client IPAdress PortNumber\n"
		"Ví dụ: ./client 127.0.0.1 5500\n\n");
		return 0;
	}

	int c_sock;
	char name[MAX], pass[MAX], buff[MAX];
	struct sockaddr_in server_addr;
	int b_sent, b_recei;

	// Xây dựng socket
	c_sock = socket(AF_INET, SOCK_STREAM, 0);

	// Xác định địa chỉ server
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	// Kiểm tra kết nối đến server
	if (connect(c_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("\nLỗi!! không thể kết nối đến máy chủ!");
		return 0;
	}

	// Giao tiếp với server
	// Nhập tên tài khoản
	puts("Hãy nhập tên tài khoản và mật khẩu của bạn: ");
	printf("Tên tài khoản: ");
	fgets(name, MAX, stdin);
	name[strcspn(name, "\n")] = '\0';

	

	while (1)
	{
		// Nhập mật khẩu
		printf("Mật khẩu: ");
		fgets(pass, MAX, stdin);
		pass[strcspn(pass, "\n")] = '\0';

		// gửi mật khẩu đến server
		if (0 >= (b_sent = send(c_sock, pass, strlen(pass), 0)))
		{
			printf("\nKết nối thất bại!\n");
			return 0;
		}

		// nhận phản hồi từ server
		memset(buff, '\0', MAX);
		if (0 >= (b_recei = recv(c_sock, buff, MAX - 1, 0)))
		{
			printf("\nLỗi!! Không nhận được phản hồi từ máy chủ!\n");
			return 0;
		}
		buff[b_recei] = '\0';
 		// nhập sai mật khẩu
		if (0 == strcmp(buff, "0"))
		{
			puts("\nSai mật khẩu. Vui lòng nhập lại(Nhập sai quá 3 lần sẽ bị khóa tài khoản)!\n");
			continue;
		}
		// nhập sai mật khẩu quá 3 lần
		else if (0 == strcmp(buff, "2"))
		{ 
			puts("\nTài khoản của bạn đã bị khóa!\n");
			return 0;
		}
		// đăng nhập thành công
		else if (0 == strcmp(buff, "1"))
		{ 
			puts("\nĐăng nhập thành công! (Nhấn enter để thoát)\n");
			getchar();
			exit(0);
		}
		else
		{
			// Lỗi phân tích dữ liệu
			puts(buff);
			puts("Lỗi máy chủ\n");
			return 1;
		}
	}

	// do
	// {
	// 	// Nhập mật khẩu
	// 	printf("Mật khẩu: ");
	// 	fgets(pass, MAX, stdin);
	// 	pass[strcspn(pass, "\n")] = '\0';

	// 	// gửi mật khẩu đến server
	// 	if (0 >= (b_sent = send(c_sock, pass, strlen(pass), 0)))
	// 	{
	// 		printf("\nKết nối thất bại!\n");
	// 		return 0;
	// 	}
	// } while (/* condition */);
	

	// Đóng socket
	close(c_sock);
	return 0;
}