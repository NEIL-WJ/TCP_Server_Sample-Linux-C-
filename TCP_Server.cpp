//============================================================================
// Name        : for_Test.cpp
// Author      : NEIL_WJ
// Version     : v1.1 'get_IP'
// Copyright   :
// Description : TCP_server/Thread/
//============================================================================

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<net/if.h>

#define recv_Len 1024
#define listen_Buff 5
#define conn_Limit 5

struct client_Slots{
	int avai_Check;
	int client_Data[conn_Limit];

};

void *tcp_Comm(void *arg){
	pthread_detach(pthread_self());

	struct sockaddr_in client_Addr;
	socklen_t client_Addr_len;

	client_Slots *client_Sock = (client_Slots*)arg;
	char test_Buff[recv_Len];
	int n, act_Slot;
	char end_Message[] = "bye";

	struct timeval recv_Time;
	fd_set recv_fd;

	printf("<tcp_Comm> start!\n");

	while(1){

		/*Check if any client connected
		 * 	if not, wait and try again*/
		if (client_Sock->avai_Check == 0){
			recv_Time.tv_sec = 1;
			recv_Time.tv_usec = 0;
			select(1,NULL,NULL,NULL,&recv_Time);	/*Block 1s, not tracking anything*/
			continue;
		}

		/*Set Select
		 * Component initialization
		 * set trigger to each accepted client socket (trigger_Number+1,write:NULL,Error:Null,read:Yes)
		 * set trigger_Number(Warning: not amount of client, but the largest client slot number)
		 * set block time*/
		FD_ZERO(&recv_fd);
		int max_nfds=0;
		for(int i=0;i<conn_Limit;i++){
			if (client_Sock->client_Data[i] != -1){
				FD_SET(client_Sock->client_Data[i],&recv_fd);
				max_nfds=(max_nfds>client_Sock->client_Data[i])?max_nfds:client_Sock->client_Data[i];
			}
		}
		recv_Time.tv_sec = 2;
		recv_Time.tv_usec = 0;
		if((act_Slot = select(max_nfds+1,&recv_fd,NULL,NULL,&recv_Time))<0){
			printf("Select error\n");
			continue;
		}

		if (act_Slot > 0){
			for(int No=0;No<conn_Limit;No++){
				if (client_Sock->client_Data[No] == -1){
					continue;
				}

				/*set receive
				 * Message_length = (target_client_socket, (char)message, Maximum_message_length, Optional_setup)*/
				if((n = recv(client_Sock->client_Data[No], test_Buff, recv_Len, 0))>0){
					test_Buff[n] = '\0';
					printf("Client[%d] Message: %s	",(No+1) , test_Buff);

					/*Get and display Client_IP:Port*/
					client_Addr_len = sizeof(client_Addr);
					getpeername(client_Sock->client_Data[No],(struct sockaddr*)&client_Addr, &client_Addr_len);
					printf("[%s:%d]\n",inet_ntoa(client_Addr.sin_addr),ntohs(client_Addr.sin_port));

					if (strcmp(test_Buff, end_Message) == 0){
						close(client_Sock->client_Data[No]);
						client_Sock->client_Data[No] = -1;
						client_Sock->avai_Check --;
						printf("client[%d] disconnected!\n",(No+1));
						printf("Remain client:%d\n",client_Sock->avai_Check);
					}
				}
				else if(n==0){
					printf("Disconnected by client[%d]\n",(No+1));
					close(client_Sock->client_Data[No]);
					client_Sock->client_Data[No] = -1;
					client_Sock->avai_Check --;
					printf("Remain client:%d\n",client_Sock->avai_Check);
				}
			}
		}
	}
}



int main(int argc, char** argv) {

	int test_Sock, thread_Check;
	struct sockaddr_in servaddr;
	char start_Message[15] = "Greeting!\n";

	struct sockaddr_in client_Addr;
	socklen_t  client_Addr_len;

	pthread_t threads;


	/* initialization variable in structure client_Slot*/
	client_Slots *client_Slot =  (client_Slots *)alloca(sizeof(client_Slots));
	client_Slot->avai_Check = 0;


	/*Set server Socket (IPv4, TCP, TCP protocol)*/
	test_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if(test_Sock == -1){
		printf("Error in creating socket!\n");
		return 0;
	}
	else{
		printf("Socket created!\n");
	}

	/*Checkout if_config and select a non-callback IP_address*/
	struct ifconf if_conf;
	struct ifreq *if_requ;
	char buf[1024];
	char server_addr[15];
	char server_name[15];
	unsigned long int if_leng = 3;
	if_conf.ifc_len = 1024;
	if_conf.ifc_buf = buf;

	ioctl(test_Sock, SIOCGIFCONF, &if_conf);
	if_requ = (struct ifreq*)buf;
	for(size_t i = 0;i < (if_conf.ifc_len/sizeof(struct ifreq)); i++)
	{
		strcpy(server_addr, inet_ntoa(((struct sockaddr_in *)&(if_requ->ifr_addr))->sin_addr));
		if(memcmp(server_addr,"127",if_leng) != 0){
			strcpy(server_name, if_requ->ifr_name);
			break;
		}
		if_requ++;
	}


	/*Set Bind
	 * initialization (=0)
	 * setup(IPv4, Server_address=Local_address, Port)*/
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server_addr);//htonl(INADDR_ANY);
	servaddr.sin_port = htons(8888);
	if (bind(test_Sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		printf("Error in bind\n");
		return 0;
	}
	else{
		printf("Bind created!\n");
	}

	if (listen(test_Sock, listen_Buff) == -1){
		printf("Error in listen!");
		return 0;
	}
	else{
		printf("Listen started!\n");
	}


	printf("Waiting for client...\n");
	printf("Server_address: [%s]%s\n",server_name, server_addr);

	/*Set structure (for_thread)
	 * Assign memory space for structure*/
	for(int i=0;i<conn_Limit;i++){
		client_Slot->client_Data[i] = -1;
	}


	/*Set thread
	 * (Thread_id, Optional_setup, Sub_function, (void *)Parameter)*/
	thread_Check = pthread_create(&threads,NULL,tcp_Comm,(void *)client_Slot);
	if (thread_Check != 0){
		printf("Error in thread create!");
		return 0;
	}


	while(1){
		if(client_Slot->client_Data[0] == -2){
			printf("-2");
			break;
		}

		int client_No = -1;
		for(int i=0;i<5;i++){
			if (client_Slot->client_Data[i] == -1){
				client_No = i;
				break;
			}
		}
		if (client_No<0){
			continue;
		}


		/*Set accept4
		 * Client_id = (Server_Socket, Optional_setup, Length_limit, Special_flag=Non_block)*/
		if((client_Slot->client_Data[client_No] = accept4(test_Sock, (struct sockaddr*)NULL, NULL, SOCK_NONBLOCK)) != -1){
			client_Slot->avai_Check ++;

			printf("Device connected. Name: client[%d]. ",client_No+1);

			/*Get and display Client_IP:Port*/
			client_Addr_len = sizeof(client_Addr);
			getpeername(client_Slot->client_Data[client_No],(struct sockaddr*)&client_Addr, &client_Addr_len);
			printf("[%s:%d]\n",inet_ntoa(client_Addr.sin_addr),ntohs(client_Addr.sin_port));

			if (client_No == 4){
				printf("Reach maximum connection number!\n");
			}

			if (send(client_Slot->client_Data[client_No],start_Message,15,0)==-1){
				printf("Error in send message\n");
				return 0;
			}
			continue;
		}
		printf("Error in accept!");
	}
	close(client_Slot->client_Data[0]);
	close(test_Sock);
	return 0;
}



