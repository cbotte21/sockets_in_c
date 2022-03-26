#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

void concat(char* buffer, char* append);
void copy(char* buffer, char* append);

int main(int argc, const char* argv[]) {
	//Predeclarations
	int socket_client;
	int connection_status;

	//Init socket
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	//Specify connection endpoint
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//Connect
	connection_status = connect(socket_client, (struct sockaddr*)&server_address, sizeof(server_address));

	//Check connection
	if (connection_status == -1) {
		printf("Connection failed!\n");
		exit(1);
	}
	
	char res[256]; //Create response buffer
	char req[101];
	char username[50];
	char password[50];

	//Get login
	printf("Username: ");
	if (!scanf(" %s", username)) {
		printf("Failed to read username!\n");
		exit(1);
	}
	
	printf("Password: ");
	if (!scanf(" %s", password)) {
		printf("Failed to read password!\n");
		exit(1);
	}
	printf("\n");

	copy(req, username);
	concat(req, "|");
	concat(req, password);

	send(socket_client, req, sizeof(req), 0);
	//Recieve data
	recv(socket_client, &res, sizeof(res), 0);

	//Print data
	printf("DATA: %s\n", res);

	//Close socket
	close(socket_client);
	return 0;
}

void concat(char* buffer, char* append) {
	for (; *buffer != '\0'; buffer++);
	for (; *append != '\0'; append++, buffer++)
		*buffer = *append;
	*buffer = *append;
}

void copy(char* buffer, char* append) {
	for (; *append != '\0'; append++, buffer++)
		*buffer = *append;
	*buffer = *append;
}
