#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *connection_handler(void* socket);
int main(int argc,char ** argv[]){
	int socket_desc,incoming_sock,c,*new_sock;
	socket_desc = socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc == -1){
		puts("could not open socket.\n");
	}else{
		puts("Going to accept requests!");	
		struct sockaddr_in server,client;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_family = AF_INET;
		server.sin_port = htons(8888);
		if(bind(socket_desc,(struct sockaddr *)&server,sizeof(server)) < 0){
			puts("failed to connect");
			return -1;
		}
		puts("bind done!");
		listen(socket_desc,3);
		c = sizeof(struct sockaddr_in);	
		while(incoming_sock = accept(socket_desc,(struct sockaddr *)&client,(socklen_t *)&c)){
			pthread_t new_thread;	
			new_sock = malloc(1);
			*new_sock = incoming_sock;
			if(pthread_create(&new_thread,NULL,connection_handler,(void*)new_sock) < 0){
				puts("Could not able to creat thread!");
				return 1;
			}	
			pthread_join(new_thread, NULL);	
		}
	}
	return 0;
	
}
void *connection_handler(void* socket){
	puts("Building connection!!!");
	int sock = *(int*) socket;
	char *message;
	message = "Greetings!I am rob!";
	write(sock,message,strlen(message));
	free(socket);
	return 0;
}
