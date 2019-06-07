#include <stdio.h>
#include <string.h>
#include <unistd.h> // close socket
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


static char savechat[255];
void *chatSync(void *arg)
{
	int socket = *((int*) arg);
    	char chat[255];
    	while(1)
	{
		recv(socket, &chat, sizeof(chat), 0);
		printf("%s", chat);
		strcat(savechat, chat);
    	}
}

int main()
{
	int networkSocket;
	networkSocket = socket(AF_INET, SOCK_STREAM, 0);

	// nurodom adresa socketui
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connectionStatus = connect(networkSocket, (struct sockaddr *) &server_address, sizeof(server_address));

	// tikrinam ar geras connectionas
	if(connectionStatus == -1)
	{
		printf("KLAIDA: Prisijungti prie socketo nepavyko!\n");
		return 0;
	}

	char client_message[64];
	recv(networkSocket, &client_message, sizeof(client_message), 0);
	char name[4] = "Bot";
	send(networkSocket, name, sizeof(name), 0);

	//uzkuriam threada kad gautume pranesimus is kitu zmoniu
	pthread_t id;
	pthread_create(&id,NULL,chatSync,&networkSocket);
	//susirasinejimas
	printf("---------------------------Susirasinejimo langas--------------------------\n");
	while(1)
	{
		sleep(20);
		pthread_cancel(chatSync);
		sleep(1);
        	send(networkSocket, savechat, sizeof(savechat), 0);
		sleep(2);
		pthread_t id;
		pthread_create(&id,NULL,chatSync,&networkSocket);
	}
	close(networkSocket);
	return 0;
}
