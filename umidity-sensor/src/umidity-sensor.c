/**
	* Temperature sensor source file
	
	* Felipe Alegria Rollo Dias		- 9293501
	* Giovanna Oliveira Guimarães	- 9293693
	* Lucas Alexandre Soares		- 9293265
	* Otavio Luiz Aguiar			- 9293518
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

#define BUFFERSIZE 1024
#define PORT 8888
#define SUFIX "\\US"

/* Display error and exit */
void die(const char *, char *, int);

int main(int argc, char const *argv[]) {
	int clientSocket, nBytes;
	char *buffer, ipAddr[20];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	printf("Enter your IP address: ");
	scanf("%s", ipAddr);

	buffer = (char *) malloc(BUFFERSIZE * sizeof(char));

  	/* Create UDP socket */
  	/**
		* AF_INET: comunication domain - IPv4 addresses
		* SOCK_DGRAM: supports datagrams (connectionless, unreliable messages
		* of a fixed maximum length)
		* IPPROTO_UDP: UDP protocol
  		*/
	if((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		die("Failed to create socket", buffer, clientSocket);

	/* Configure settings in address structc */
	/**
		* htons(): converts integer from host byte order to network byte order
		* (little-endian to big-endian)
		* inet_aton: converts IPv4 notation into binary data in network byte order
		* I don't know what sin_zero does. Fuck dat shit
	*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_aton(ipAddr, &serverAddr.sin_addr);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	addr_size = sizeof serverAddr;

	while(true) {
		scanf(" %[^\n]s", buffer);

	    /* TS + Message */
	    strcat(buffer, SUFIX);

	    nBytes = strlen(buffer) + 1;
	    
	    /* Send message to server */
	    if((sendto(clientSocket, buffer, nBytes, 0, (const struct sockaddr *)&serverAddr, addr_size) < 0))
	    	die("Failed to send data", buffer, clientSocket);

	    /* Receive message from server */
	    if((nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL) < 0))
	    	die("Failed to get data", buffer, clientSocket);

	    printf("Received from server: %s\n",buffer);
	    sleep(1);
  	}

  	free(buffer);
	
	return 0;
}

void die(const char *errorMsg, char *buffer, int clientSocket) {	
	if(clientSocket != -1)
		close(clientSocket);
    
    perror(errorMsg);
    free(buffer);
    exit(errno);
}