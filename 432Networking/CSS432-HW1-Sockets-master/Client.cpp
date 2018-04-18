#include <stdio.h> //printf
#include <stdlib.h> //atoi
#include <unistd.h> //read, write, close
#include <sys/types.h> //socket, bind
#include <sys/socket.h> //socket, bind, listen, inet_ntoa
#include <netinet/in.h> //htonl, htons, inet_ntoa 

#include <arpa/inet.h> //inet_ntoa
#include <netdb.h> //gethostbyname
#include <string.h> //bzero
#include <netinet/tcp.h> //SO_REUSEADDR
#include <sys/uio.h> //writev
#include <time.h> //time

/**
* Input format:
* argv[1] port:			server IP port
* argv[2] repetition	repetition of sending a set of data buffer
* argv[3] nbufs: 		the number of data buffers
* argv[4] bufsize: 		the size of each data buffer(in bytes)
* argv[5] type:			the type of transfer scenario: 1, 2, or 3

*/
int main(int argc, char* argv[]) {

	//input checking
	if(argc < 5) { 
		fprintf(stderr, "ERROR: port, repetition, nbufs, bufsize, and type arguments required");
		return(-1);
	}

	//Local vars
	char ip[] = "uw1-320-13.uwb.edu"; //hardcoded IP address
	int server_port = atoi(argv[1]);
	int repetition = atoi(argv[2]);
	int nbufs = atoi(argv[3]);
	int bufsize = atoi(argv[4]);
	int type = atoi(argv[5]);
	char * databuf[nbufs]; //buffer

	int socket_fd; //aka client
	struct sockaddr_in server_addr; //sockaddr_in holds an ip address 
	struct hostent* server; //defines a host computer on the internet

	bzero((char*) &server_addr, sizeof(server_addr)); //zero initialize sockaddr_in struct
	//server = gethostbyname(argv[2]); 

	//Set up server settings
	server_addr.sin_family = AF_INET; //address family internet
	server_addr.sin_addr.s_addr = 
		inet_addr( inet_ntoa( *(struct in_addr*)*server->h_addr_list )); //?
	server_addr.sin_port = htons(server_port); //?
	socket_fd = socket(AF_INET, SOCK_STREAM, 0); //open a stream oriented socket
	connect(socket_fd, (sockaddr*) &server_addr, sizeof(server_addr));

	//record time before and after writing and reading
	time_t tBefore, tAfter, tDiff;
	tBefore = time(NULL);

	//write and read
	switch(type) {
		case 1: { //multiple writes
			for(int i = 0; i < repetition; i++) {
				//need to flush BUFFER BETWEEN SENDS
				for(int j = 0; j < nbufs; j++) {
				write(socket_fd, databuf[j], bufsize);
				
				}	
			}
			
		}
		case 2: { //writev
			for(int i = 0; i < repetition; i++) {
				//need to flush BUFFER BETWEEN SENDS
				struct iovec vector[nbufs];
				for(int j = 0; j < nbufs; j++) {
					vector[j].iov_base = databuf[j];
					vector[j].iov_len = bufsize;
				}
				write(socket_fd, vector, nbufs);
			}
			
		}
		case 3: { //single write
			//need to flush BUFFER BETWEEN SENDS
			for(int i = 0; i < repetition; i++) {
				write(socket_fd, databuf, nbufs * bufsize);
			}
			
		}
		default: {
			fprintf(stderr, "ERROR: invalid write type");
			return(-1);
		}
	}
	tAfter = time(NULL);
	tDiff = tAfter - tBefore;

	//need to print more statistics
	fprintf(stdout, "Seconds elapsed: %lu", (unsigned long)tDiff);

	close(socket_fd); //close the socket

	return 0;
}