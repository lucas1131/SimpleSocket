/* Master sensor source file

	Felipe Alegria Rollo Dias	- 9293501
	Giovanna Oliveira Guimarães	- 9293693
	Lucas Alexandre Soares		- 9293265
	Otavio Luis de Aguiar		- 9293518
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

#include <pthread.h>

#define IPV4	 	AF_INET
#define DATAGRAM	SOCK_DGRAM
#define PORT 		8888
#define BUFFSIZE 	1024

#define ANY_ADDR 		INADDR_ANY
#define LOOPBACK_ADDR 	INADDR_LOOPBACK
#define BROADCAST_ADDR 	INADDR_BROADCAST

using namespace std;

int fuel;
int humidity;
int last_humidity;
double pressure;
double last_pressure;
double temperature;
double last_temperature;
int gpsa;
double gpsla;
double gpslo;
int rgpsa;
double rgpsla;
double rgpslo;
double last_gpsla;
double last_gpslo;
double remaining_time;
bool turbulence;
string direction;
bool enough_fuel;

// void die(const char *s, char *data, );
void die(const char *s);
void ProcessData(char *data, int length);

	int udp_socket = -1;			// Socket Descriptor
	int terminal_thread = -1;		// Terminal thread descriptor
    pthread_t *thread;
	char *data;						// Data buffer
int main(int argc, char **argv){


    struct sockaddr_in socket_addr;	// Server
    struct sockaddr_in client_addr; // Clients
    socklen_t client_size = sizeof(client_addr);

    int data_length;

	data = (char *) malloc(sizeof(char)*BUFFSIZE);

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

    // Create a user interface in seperate
    // thread_descriptor = pthread_create(thread, NULL, &ListenLocalCommand, NULL);

    // Keep listening for data forever
    while(true){

        fflush(stdout); // Force flush from stdout internal buffering

        // Clear data data before getting more
        memset(data, 0, BUFFSIZE);
        // Get data - blocking call as scanf or getchar etc
        if( (data_length = recvfrom(udp_socket, data, BUFFSIZE,
        							0, (struct sockaddr *)
        							&client_addr, &client_size) ) < 0)
            fprintf(stderr, "Failed to get data");

        // Print sender IP:PORT and data
        printf("Received packet from %s:%d\n",
        	inet_ntoa(client_addr.sin_addr),
        	ntohs(client_addr.sin_port));

        printf("Data: %s\n" , data);
        ProcessData(data, data_length);

    }

    close(udp_socket);
	return 0;
}

// Display error and exit
void die(const char *s){

	if(udp_socket != -1)
		close(udp_socket);

    perror(s);
    free(data);
    exit(errno);
}

void ProcessData(char *data, int length) {
	char c;
	int pos = 0;

	while(data[pos] != '\\') pos++;

	string sdata(data, pos);	//string that contains the data

	++pos;

	//detects the sensor the data came from
	switch(data[pos]) {
		case 'F':
			fuel = stoi(sdata, NULL, 10);
			break;
		case 'G':
			pos += 3;
			if (data[pos] == 'A')
				gpsa = stoi(sdata, NULL, 10);
			else {
				++pos;
				if (data[pos] == 'A')
					gpsla = stod(sdata, NULL);
				else
					gpslo = stod(sdata, NULL)
			}
			break;
		case 'H':
			humidity = stoi(sdata, NULL, 10);
			break;
		case 'P':
			pressure = stod(sdata, NULL);
			break;
		case 'R':
			pos += 4;
			if (data[pos] == 'A')
				rgpsa = stoi(sdata, NULL, 10);
			else {
				++pos;
				if (data[pos] == 'A')
					rgpsla = stod(sdata, NULL);
				else
					rgpslo = stod(sdata, NULL)
			}
			break;
		case 'T':
			temperature = stod(sdata, NULL);
			break;
	}
}

void UpdateRemainingTime() {
	double speed;
	double distance;

	//length in meters of a degree
	meter_p_degree_lat = 111132.954 - 559.822 * cos(2 * gpsla) + 1.175 * cos(4 * gpsla);
	meter_p_degree_lon = 111132.954 * cos(gpslo);
	//calculate speed (0.5s between data)
	speed = sqrt(meter_p_degree_lat*fabs(gpsla - last_gpsla)*fabs(gpsla - last_gpsla) + eter_p_degree_lon*fabs(gpslo - last_gpslo)*fabs(gpslo - last_gpslo));
	speed /= 1000;
	speed /= 0.5;

	//calculate distance to arrival
	distance = sqrt(eter_p_degree_lat*fabs(gpsla - rgpsla)*fabs(gpsla - rgpsla) + eter_p_degree_lon*fabs(gpslo - rgpslo)*fabs(gpslo - rgpslo));
	distance /= 1000;

	remaining_time = distance/speed;
}

void UpdateTurbulence() {
	turbulence = false;

	if (fabs(pressure - last_pressure) > 0.5)
		turbulence = true;

	if (fabs(temperature - last_temperature) > 5.0)
		turbulence = true;

	if (fabs(humidity - last_humidity) > 10)
		turbulence = true;

	last_pressure = pressure;
	last_temperature = temperature;
	last_humidity = humidity;
}

void UpdateDirection() {
	bool up = false;
	bool right = false;

	if (gpsla - last_gpsla > 0)
		up = true;

	if (gpslo - last_gpslo > 0)
		right = true;

	if (up) {
		if (right)
			direction = "North East";
		else
			direction = "North West";
	} else {
		if (right)
			direction = "South East";
		else
			direction = "South West";
	}

	last_gpsla = gpsla;
	last_gpslo = gpslo;
}

void UpdateEnoughFuel() {
	enough_fuel = false;
	double consumption = 8.09;

	//calculate distance to arrival
	distance = sqrt(eter_p_degree_lat*fabs(gpsla - rgpsla)*fabs(gpsla - rgpsla) + eter_p_degree_lon*fabs(gpslo - rgpslo)*fabs(gpslo - rgpslo));
	distance /= 1000;

	if (distance*consumption < fuel)
		enough_fuel = true;
}

void *ListenLocalCommand(void *foo){

	char *op = (char *) malloc(sizeof(char)*1024);

	do{
		scanf("%[^\n]s", op);
		if(!strcmp(op, "show gps"))
			printf("Altitude: %f\nLatitude: %f\nLongitude: %f");
		else if(!strcmp(op, "show temperature"))
			printf("");
		else if(!strcmp(op, "show pressure"))
			printf("");
		else if(!strcmp(op, "show fuel"))
			printf("");
		else if(!strcmp(op, "show humidity"))
			printf("");
	} while(strcmp(op, "q"));

	return op;
}
