#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

int main(int argc, char *argv[]){
	int server_socket client_socket;
	struct sockaddr_in server_addr, client_addr;
	int client_addr_size;
	char buf[BUFSIZ];
	
	if(argc != 2){
		printf("잘못 입력했습니다.\n");
		exit(1);
	}
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1){
		printf("server socket fail\n");
		exit(1);
	}
	printf("server socket open\n");
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		printf("bind error\n");
		exit(1);
	}
	printf("bind success\n");

	if(listen(server_socket, 5) == -1){
		printf("listen fail\n");
		exit(1);
	}
	printf("listen success\n");
	
	int client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t *)(&client_addr_size));
	if(client_socket == -1){
		printf("client socket fail\n");
		exit(1);
	}
	printf("connected client: %d \n", client_socket);
	printf("Start...\n");
	int rcv = recv(client_socket, message, sizeof(message)-1, 0);
	if(rcv == -1){
		printf("Receive Error\n");
		exit(1);
	}
	printf("Receive Message : %s\n", message);
	char *start_mes = strstr(message, "Hose: ");
	char *end_mes = strstr(message, "\x0d\x0a");
	start_mes += 6;
	char URL[BUFSIZ], server_ip[30];
	
	strncpy(URL, start_mes, end_mes - start_mes);
	
	struct hostent *host;
	struct in_addr **addr_list;
	
	host = gethostbyname(URL);
	addr_list = (struct in_addr **) host->h_addr_list;
	strcpy(server_ip, inet_ntoa(*addr_list[0]));
	printf("URL : %s\n ip : %s", URL, server_ip);
	
	int server_socket1;
	struct sockaddr_in server_addr1;
	server_socket1 = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1){
		printf("server1 socket fail\n");
		exit(1);
	}
	printf("server1 socket open\n");
	memset(&server_addr1, 0, sizeof(server_addr1));
	server_addr1.sin_family	= AF_INET;
	server_addr1.sin_port	= htons(atoi(server_ip));
	server_addr1.sin_addr.s_addr	= htonl(atoi("80"));
		
	if(connect(server_socket1, (struct sockaddr *)&server_addr1, sizeof(server_addr1)) == -1){
		printf("connect error\n");
		exit(1);
	}
	printf("connect success!\n");
	
	send(server_socket1, message, (int)strlen(message), 0);
	memset(message, 0, sizeof(message));
	recv(server_socket1, message, sizeof(message));
	printf("message : %s\n", message);
	send(client_socket, message, (int)strlen(message), 0);
	
	close(client_socket);
	close(server_socket1);
	close(server_socket);
	
	
}
