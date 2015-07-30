#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> /*header file contains constants and structures needed for internet domain addresses.*/

/*function called when a system call fails. It displays a message about the error on stderr and then aborts the program.*/
void error(const char *msg)
{
    perror(msg); 
    exit(1);
}


/*To run the client you need to pass in port number. */
int main(int argc, char *argv[])
{
     /*sockfd and newsockfd are file descriptors
     portno stores the port number on which the server accepts connections.
     clilen stores the size of the address of the client. 
     */
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256]; /*The server reads characters from the socket connection into this buffer.*/
     struct sockaddr_in serv_addr, cli_addr; /*A sockaddr_in is a structure containing an internet address.*/
     int n; /*n is the return value for the read() and write() calls*/
     
     /*The user needs to pass in the port number on which the server will accept connections as an argument. This code displays an error message if the user fails to do this.*/
     if (argc < 2) {
         fprintf(stderr,"Error, You should enter port number which larger than 2000.\n");
         exit(1);
     }
    
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
    
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     /*The bind() system call binds a socket to an address, in this case the address of the current host and port number on which the server will run.*/
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
    newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
    
     n = write(newsockfd,"Server: I got client's message",30);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
