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
#include<sys/queue.h>

#define recv_Len 1024
#define listen_Buff 5
#define conn_Limit 5



typedef struct mess{
	CIRCLEQ_ENTRY(mess) field;
	char *message;
	//int message;
}mess,et;

typedef CIRCLEQ_HEAD(mess_head,mess) mess_head,et_head;


int main(int argc, char** argv) {

	mess_head *p_head = malloc(sizeof(mess_head));
	CIRCLEQ_INIT(p_head);

	mess *elm;


	elm = malloc(sizeof(mess));
	elm->message = malloc(sizeof(1));
	strcpy(elm->message,"a");
	CIRCLEQ_INSERT_HEAD(p_head,elm,field);

	elm = malloc(sizeof(mess));
	elm->message = malloc(sizeof(1));
	strcpy(elm->message,"befdgtnhehhrthrssdweg");
	CIRCLEQ_INSERT_HEAD(p_head,elm,field);

	elm = malloc(sizeof(mess));
	elm->message = malloc(sizeof(1));
	strcpy(elm->message,"c");
	CIRCLEQ_INSERT_HEAD(p_head,elm,field);

	while(1){
		if(CIRCLEQ_FIRST(p_head)==CIRCLEQ_LAST(p_head)){
			break;
		}
		char a[15];
		strcpy(a,CIRCLEQ_LAST(p_head)->message);
		printf("%s\n-----\n",a);
		elm = CIRCLEQ_LAST(p_head);
		CIRCLEQ_REMOVE(p_head,elm,field);

	}
	char a[15];
	strcpy(a,CIRCLEQ_LAST(p_head)->message);
	printf("%s\n-----\n",a);
	elm = CIRCLEQ_LAST(p_head);
	CIRCLEQ_REMOVE(p_head,elm,field);




	CIRCLEQ_FOREACH(elm,p_head,field){
		printf("%s\n",elm->message);
		CIRCLEQ_REMOVE(p_head,elm,field);
	}

	printf("-------------\n");


	et_head *e_head = malloc(sizeof(et_head));
	CIRCLEQ_INIT(e_head);

	et *e, *e1, *e2, *e3;

	e = malloc(sizeof(mess));
	e->message = malloc(sizeof(1));

	e1 = malloc(sizeof(mess));
	e1->message = malloc(sizeof(1));
	strcpy(e1->message,"1");
	CIRCLEQ_INSERT_HEAD(e_head,e1,field);
	e2 = malloc(sizeof(mess));
	e2->message = malloc(sizeof(1));
	strcpy(e2->message,"2");
	CIRCLEQ_INSERT_HEAD(e_head,e2,field);
	e3 = malloc(sizeof(mess));
	e3->message = malloc(sizeof(1));
	strcpy(e3->message,"3");
	CIRCLEQ_INSERT_HEAD(e_head,e3,field);

	strcpy(e1->message,"g");

	CIRCLEQ_FOREACH(e,e_head,field){
		printf("%s\n",e->message);
	}




	printf("end\n");
	return 0;
}



