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

int server_fd, new_server_fd, repetition;
const int BUFSIZE = 1500;

void my_function(int signal_id) {
	time_t tBefore, tAfter, tDiff;
	char * databuf[BUFSIZE];

	tBefore = time(NULL); //time before read
	int count = 0;

	//receive data from client
	for(int i = 0; i < repetition; i++) {
		//read from buffer
		for(int numRead = 0; numRead < BUFSIZE; count++) {
			int readlen = read(new_server_fd, databuf, BUFSIZE - numRead);
			numRead += readlen;
		}
		count++;
	}
	tAfter = time(NULL); //time after read
	tDiff = tAfter - tBefore; 

	write(new_server_fd, &count, sizeof(count));

	//close scoket
	close(new_server_fd);
}

/**
* Input format:
* argv[1] port:			server IP port
* argv[2] repetition:	repetition of sending a set of data buffer
*
*/
int main(int argc, char* argv[]) {

	if(argc < 2) {
		fprintf(stderr, "ERROR: port and repetition arguments required\n");
		return(-1);
	}

	//declare sockaddr_in structure
	struct sockaddr_in acceptSockAddr;
	//zero initialize sockaddr_in struct
	bzero((char*) &acceptSockAddr, sizeof(acceptSockAddr)); 

	//necessary variables
	int server_port = atoi(argv[1]); // the last 5 digits of your student id
	repetition = atoi(argv[2]);
	
	//set up server settings
	acceptSockAddr.sin_family = AF_INET; //address family internet
	acceptSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	acceptSockAddr.sin_port = htons(server_port); 

	//open stream oriented socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0); 
	fprintf(stdout, "Socket established\n");

	//set SO_REUSEADDR option
	const int on = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int));

	//bind socket
	bind(server_fd, (sockaddr*)&acceptSockAddr, sizeof(acceptSockAddr));
	fprintf(stdout, "Port Bound\n");

	//instruct OS to listen to up to 5 connection requests
	listen(server_fd, 5);
	fprintf(stdout, "Listening for Connections\n");

	//receive a request from client by calling accept that will return a new socket
	//specific to this connection request
	sockaddr_in newServerAddr;
	socklen_t newSockAddrSize = sizeof(newServerAddr);
	int new_server_fd = accept(server_fd, (sockaddr*)&new_server_fd,&newSockAddrSize);


	return 0;
}