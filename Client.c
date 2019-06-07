#include <stdio.h>
#include <string.h>
#include <unistd.h> // close socket
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *chatSync(void *arg)
{
	int socket = *((int*) arg);
	char chat[100];
	while(1)
	{
		recv(socket, &chat, sizeof(chat), 0);
		printf("%s", chat);
	}
}

int main(){
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
    printf("%s \n", client_message);
    printf("Įvesk savo varda ... --> ");
    char name[20];
    fgets (name, 20, stdin);
    strtok(name, "\n");
    send(networkSocket, name, sizeof(name), 0);

    //uzkuriam threada kad gautume pranesimus is kitu zmoniu
    pthread_t id;
    pthread_create(&id,NULL,chatSync,&networkSocket);
    //susirasinejimas
    printf("---------------------------Susirasinejimo langas--------------------------\n");
    while(1){
        char server_message[64];
        fgets (server_message, 256, stdin);
        send(networkSocket, server_message, sizeof(server_message), 0);
    }
    close(networkSocket);
    return 0;

}
