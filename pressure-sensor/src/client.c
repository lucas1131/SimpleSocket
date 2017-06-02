#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[]) {
	int clientSocket, nBytes;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

  	/* Create UDP socket */
  	/**
		* AF_INET: comunication domain - IPv4 addresses
		* SOCK_DGRAM: supports datagrams (connectionless, unreliable messages
		* of a fixed maximum length)
		* IPPROTO_UDP: UDP protocol
  		*/
	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* Configure settings in address structc */
	/**
		* htons(): converts integer from host byte order to network byte order
		* (little-endian to big-endian)
		* inet_aton: converts IPv4 notation into binary data in network byte order
		* I don't know what sin_zero does. Fuck dat shit
	*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	inet_aton("127.0.0.1", &serverAddr.sin_addr);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	addr_size = sizeof serverAddr;

	while(1){
	    printf("Type something: ");
	    fgets(buffer,1024,stdin);

	    nBytes = strlen(buffer) + 1;
	    
	    /* Send message to server */
	    sendto(clientSocket, buffer, nBytes, 0, (const struct sockaddr *)&serverAddr, addr_size);

	    /*Receive message from server*/
	    nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);

	    printf("Received from server: %s\n",buffer);
  	}
	
	return 0;
}