/*
 * 1. Creat socket with socket() syscall
 * 2. Bind socket to an address with bind() syscall (host, port)
 * 3. Listen for connections with listen() syscall
 * 4. Accept connection (blocking) with accept() syscall
 * 5. Send and receive data
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in server, client;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	/* Error handler */
	void handle_error(char* type) {
		return perror(type);
		exit(1);
	}

	/* Call socket() */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		handle_error("creation");
	}

	/* Initiliaze socket structure */
	//bzero((char *) &serv_addr, sizeof(serv_addr)); /* Zero-out the socket structure in memory */
	memset(&serv_addr, 0, sizeof(struct sockaddr_in)); // Turns out memset is more standard than weirdo bzero
	portno = 1337;

	/* Re-populate the socket structure with good values */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno); // htons() keeps bytes in order they are transmitted, overriding endianness rules

	/* Bind to the host */
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(struct sockaddr_in)) == -1)
		handle_error("bind");

	/* Listen (blocking call) */
	listen(sockfd, 5);

	clilen = sizeof(cli_addr);

	/* Accept a connection */
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd == -1) {
		handle_error("accept");
	}

	/* Connection established, start communication */
	memset(buffer, 0, sizeof(buffer));
	n = read(newsockfd, buffer, 255);
	if (n == -1) {
		handle_error("read");
	}

	printf("Here is the message I received: %s\n", buffer);

	/* Send a response */
	n = write(newsockfd, "PONG!\n", 7);
	if (n < 0) {
		handle_error("Write error");
	}

	return 0;

}
