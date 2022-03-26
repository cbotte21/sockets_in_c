#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define MAX_POOL_SIZE 5
#define BUFFER_SIZE 256

#define USERNAME "cbotte21"
#define PASSWORD "asdfasdf1"

typedef enum {FALSE, TRUE} Boolean;

//Prototypes for validation
int compareStrings(char* persistent, char* freed); //Strings are equal
char* getUsername(char* string, char* username);
char* getPassword(char* string, char* password);

int main(int argc, const char* argv[]) {
	//Predeclarations
	int socket_server, client_socket;
	char authed_message[] = "User is authenticated.";
	char unauthed_message[] = "Failed to authenticate.";

	//Initialize socket
	socket_server = socket(AF_INET, SOCK_STREAM, 0);

	//Specify connection endpoint
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//Set address
	bind(socket_server, (struct sockaddr*)&server_address, sizeof(server_address));

	//Start server
	listen(socket_server, MAX_POOL_SIZE);

	printf("Socket server started!\n");

	//Data buffer
	Boolean authed;
	char buffer[256];
	char username[50];
	char password[50];
	//Store connection
	while(1) {
		//Incoming sockets
		client_socket = accept(socket_server, NULL, NULL); //Can whitelist connection location
		read(client_socket, buffer, BUFFER_SIZE);
		printf("Socket connected! DATA: { %s }\n", buffer);
		if (compareStrings(USERNAME, getUsername(buffer, username)))
			if (compareStrings(PASSWORD, getPassword(buffer, password)))
				authed = 1;
		//Send message
		if (authed)
			send(client_socket, authed_message, sizeof(authed_message), 0);
		else
			send(client_socket, unauthed_message, sizeof(unauthed_message), 0);
		//Close socket
		close(client_socket);
		authed = 0;
	}

	close(socket_server);
	return 0;
}

int compareStrings(char* persistent, char* freed) {
	int res = 0;
	for (; *persistent == *freed && *freed == '\0'; persistent++, freed++)
		res = 1;
	return res;
}

char* getUsername(char* string, char* username) {
	for(int i = 0; string[i] != '|' && i < 50; i++)
		username[i] = string[i];
	return username;
}

char* getPassword(char* string, char* password) {
	int i;
	while(*string != '|')
		string++;
	for (i = 0; string[i] != '\0' && i < 50; i++)
		password[i] = string[i];
	password[i] = string[i];
	return password;
}
