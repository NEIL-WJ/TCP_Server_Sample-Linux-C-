//============================================================================
// Name        : TCP_Server
// Author      : NEIL-WJ
// Version     : v1.0 
// Summary     : Can accept only one client, Blocking mode(low cpu usage)
// Description : Basic TCP server (can only communicate with one client a time)
// Update	   : First version
//============================================================================

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define recv_Len 1024
#define listen_Buff 5


int main(int argc, char** argv) {
	int test_Sock, client_Sock;
	struct sockaddr_in servaddr;
	char test_Buff[recv_Len];
	int n;

	char end_Message[] = "bye";

	test_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if(test_Sock == -1){
		printf("Error in creating socket!\n");
		return 0;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8888);
	if (bind(test_Sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		printf("Error in bind\n");
		return 0;
	}

	if (listen(test_Sock, listen_Buff) == -1){
		printf("Error in listen!");
		return 0;
	}

	printf("Waiting for client...\n");
	while(1){
		if((client_Sock = accept(test_Sock, (struct sockaddr*)NULL, NULL)) == -1){
			printf("Error in accept!");
			continue;
		}
		printf("Device connected!\n");
		while((n = recv(client_Sock, test_Buff, recv_Len, 0))>0){
			test_Buff[n] = '\0';
			printf("Message: %s\n", test_Buff);
			if (strcmp(test_Buff, end_Message) == 0){
				printf("Client disconnect!\n");
				close(client_Sock);
			}
		}
	}
	close(test_Sock);
	return 0;
}
