/* Master sensor source file 
	
	Felipe Alegria Rollo Dias	- 9293501
	Giovanna Oliveira Guimarães	- 9293693
	Lucas Alexandre Soares		- 9293265
	Otavio Luiz Aguiar			- 9293518
*/

// Std's
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Errors
#include <errno.h>

// Networking includes
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#define IPV4	 	AF_INET
#define DATAGRAM	SOCK_DGRAM
#define PORT 		8888
#define BUFFSIZE 	1024

#define ANY_ADDR 		INADDR_ANY
#define LOOPBACK_ADDR 	INADDR_LOOPBACK
#define BROADCAST_ADDR 	INADDR_BROADCAST

char *buffer;
int udp_socket = -1;			// Socket Descriptor

void die(const char *s);

int main(int argc, char **argv){

    struct sockaddr_in socket_addr;	// Server
    struct sockaddr_in client_addr; // Clients
    socklen_t client_size = sizeof(client_addr);
    int data_length;

	// Allocate global server buffer
	buffer = (char *) malloc(sizeof(char)*BUFFSIZE);

	if((udp_socket = socket(IPV4, DATAGRAM, 0)) < 0)
		die("Failed to create socket");

	// Initialize server socket
    memset((char *) &socket_addr, 0, sizeof(socket_addr));
     
    socket_addr.sin_family = IPV4;
    socket_addr.sin_port = htons(PORT);
    socket_addr.sin_addr.s_addr = htonl(ANY_ADDR); // Receive from any address
     
    // Bind server socket to port
    if(bind(udp_socket, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) < 0)
        die("Failed to bind server socket");
     
    // Keep listening for data forever
    while(true){
     
        fflush(stdout); // Force flush from stdout internal buffering
        
        // Get data - blocking call as scanf or getchar etc
        if( (data_length = recvfrom(udp_socket, buffer, BUFFSIZE, 
        							0, (struct sockaddr *) 
        							&client_addr, &client_size) ) < 0)
            die("Failed to get data");
         
        // Print sender IP:PORT and data
        printf("Received packet from %s:%d\n", 
        	inet_ntoa(client_addr.sin_addr),
        	ntohs(client_addr.sin_port));

        printf("Data: %s\n" , buffer);
         
        // Now reply the client with the same data
        if(sendto(udp_socket, buffer, data_length, 0, 
        		(struct sockaddr*) &client_addr, client_size) < 0)
            die("Failed to send data");
    }
 
    close(udp_socket);
	return 0;
}

// Display error and exit
void die(const char *s){
	
	if(udp_socket != -1) 
		close(udp_socket);
    
    perror(s);
    free(buffer);
    exit(errno);
}