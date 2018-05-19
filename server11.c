/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */

 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <string.h>
 #include <netdb.h>
 #include <sys/types.h> 
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 
 #define BUFSIZE 1024
 #define PORTNO 10010   /* port to listen on */
 /*
  * error - wrapper for perror
  */
 void error(char *msg) {
   perror(msg);
   exit(1);
 }
 
 int main(int argc)//, char **argv) {
     {
     int sockfd; /* socket */
     int counter; // counter to chech if any packet is lost
     int count; // counts missing packest
   //int portno; 
   int clientlen; /* byte size of client's address */
   struct sockaddr_in serveraddr; /* server's addr */
   struct sockaddr_in clientaddr; /* client addr */
   struct hostent *hostp; /* client host info */
   char buf[BUFSIZE]; /* message buf */
   char *hostaddrp; /* dotted decimal host addr string */
   int optval; /* flag value for setsockopt */
   int n, n2; /* message byte size */
 
   counter = 0;
   count = 0;
   /* 
    * check command line arguments 
    */
   if (argc != 1) {
     fprintf(stderr, "usage: %s <port>\n");
     exit(1);
   }
   //portno = atoi(argv[1]);
 
   /* 
    * socket: create the parent socket 
    */
   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   if (sockfd < 0) 
     error("ERROR opening socket");
 
   /* setsockopt: Handy debugging trick that lets 
    * us rerun the server immediately after we kill it; 
    * otherwise we have to wait about 20 secs. 
    * Eliminates "ERROR on binding: Address already in use" error. 
    */
   optval = 1;
   setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
          (const void *)&optval , sizeof(int));
 
   /*
    * build the server's Internet address
    */
   bzero((char *) &serveraddr, sizeof(serveraddr));
   serveraddr.sin_family = AF_INET;
   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
   serveraddr.sin_port = htons((unsigned short)PORTNO);
 
   /* 
    * bind: associate the parent socket with a port 
    */
   if (bind(sockfd, (struct sockaddr *) &serveraddr, 
        sizeof(serveraddr)) < 0) 
     error("ERROR on binding");
 
   /* 
    * main loop: wait for a datagram, then echo it
    */
   clientlen = sizeof(clientaddr);
   while (1) {
 
     /*
      * recvfrom: receive a UDP datagram from a client
      */
     bzero(buf, BUFSIZE);
     n = recvfrom(sockfd, buf, BUFSIZE, 0,(struct sockaddr *) &clientaddr, &clientlen);
     
     if (n < 0)
     {
         error("ERROR in recvfrom");
         
     }
       
     else 
     {
       counter = counter + 1;
       printf("\n Number of strings recieved = %d \n", counter);
     }
 
     /* 
      * gethostbyaddr: determine who sent the datagram
      */
     hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
               sizeof(clientaddr.sin_addr.s_addr), AF_INET);
     if (hostp == NULL)
       error("ERROR on gethostbyaddr");
     hostaddrp = inet_ntoa(clientaddr.sin_addr);
     if (hostaddrp == NULL)
       error("ERROR on inet_ntoa\n");
     printf("server received datagram from %s (%s)\n", 
        hostp->h_name, hostaddrp);
     printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);
     
     /* 
      * sendto: echo the input back to the client 
      */
     n2 = sendto(sockfd, buf, strlen(buf), 0, 
          (struct sockaddr *) &clientaddr, clientlen);
          printf("server sent = %s\n",buf );
          count = count + 1;
          printf("\n count = %d \n", count);
     if (n2 < 0) 
       error("ERROR in sendto");
   }
   
 }