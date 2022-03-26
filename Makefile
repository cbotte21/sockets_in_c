CC = gcc
CFLAGS = -Wall --std=c99

client: client.c
	$(CC) $(CFLAGS) client.c -o client.exe
server: server.c
	$(CC) $(CFLAGS) server.c -o server.exe
all:
	make client && make server
clean:
	rm -f client.exe server.exe
