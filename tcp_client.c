#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void handle_error(char *error_type) {
	return perror(error_type);
	exit(1);
}

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serveraddr;
	char buffer[256];
	struct hostent *server;
	char *hostname;

	if (argc != 3) {
		fprintf(stderr, "Insufficient arguments\n");
		exit(0);
	}

	hostname = argv[1];
	portno = atoi(argv[2]);


	/* Create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		handle_error("creation");
	}
	
	/* Get server hostname */
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr, "Host %s does not exist\n", hostname);
		exit(0);
	}

	/* Server INET address */
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	memmove((char *) &serveraddr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);
	serveraddr.sin_port = htons(portno);

	/* Connect */
	if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0)
		handle_error("connection");

	/* Read message from user */
	memset(buffer, 0, sizeof(buffer));
	fgets(buffer, sizeof(buffer), stdin);

	/* Relay message to server */
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0) {
		handle_error("write");
	}

	/* Read and print server reply */
	memset(buffer, 0, sizeof(buffer));
	n = read(sockfd, buffer, strlen(buffer));
	if (n < 0) {
		handle_error("read error");
	}
	printf("Echo from server %s\n", buffer);
	close(sockfd);
	return 0;
}
