#include <stdio.h>
#include <string.h>
#include <unistd.h> // close socket
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

struct threadParams {                                                   
    int socket;
    int sockets[100];                                                             
    int count;
    int message                                                           
};

void *valdymas(void *arg){
    //int socket = *((int*) soketas);
    struct threadParams *tParams = arg;
    int socket = tParams->socket;
    char vardas[20] = {0};
    char request[] = "ATSIUSKVARDA";
    //kliento vardo uzregistravimas

        send(socket, request, 64, 0);
        if(recv(socket, &vardas, sizeof(vardas), 0) == 0){
            return;
        }
        if(vardas == NULL){
            return;
        }
        printf("User %s logged in!\n", vardas);

    while(1){
        char client_message[255] = {0};
        if(recv(socket, &client_message, sizeof(client_message), 0) == 0){
            return;
        }
        tParams->message++;
        printf("%d: PRANESIMAS %s: %s",tParams->message, vardas, client_message);

        
        char message_to_all[255];
        sprintf(message_to_all, "PRANESIMAS %s: %s", vardas, client_message);
        for(int i = 0; i < tParams->count; ++i){
		//if (i != 2)
            		send(tParams->sockets[i], message_to_all, 255, 0);
        }
    }
}


int main(){
    printf("Serveris veikia...");


    int serverSocket;
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// nurodom serverio adresa
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// prikalam socketa prie IP ir porto
	bind(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address));
    listen(serverSocket, 5);
    struct threadParams *tParams;
    tParams = malloc(sizeof(*tParams));
    tParams->count = 0;
    tParams->message = 0;

    while(1){
	    tParams->socket = accept(serverSocket, NULL, NULL);
        tParams->sockets[tParams->count] = tParams->socket;
        pthread_t id;
        pthread_create(&id,NULL,valdymas,tParams);
        //pthread_join(id, NULL);
        tParams->count++;
    }

    close(serverSocket);
    return 0;

}
