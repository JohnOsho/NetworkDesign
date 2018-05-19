/* 
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h> 
 
 #define BUFSIZE 1024
 #define PORTNO 10010   /* port to listen on */
 /* 
  * error - wrapper for perror
  */
 void error(char *msg) {
     perror(msg);
     exit(0);
 }
 
 int main(int argc, char **argv) {
     int sockfd, n;
     int serverlen;
     struct sockaddr_in serveraddr;
     struct hostent *server;
     char *hostname;
     char buf[BUFSIZE];
     struct timeval start,end;
     double t1,t2;
 
     t1=0.0; t2=0.0;
     /* check command line arguments */
     if (argc != 2) {
        fprintf(stderr,"usage: %s <hostname> \n", argv[0]);
        exit(0);
     }
     hostname = argv[1];
     //portno = atoi(argv[2]);
 
     /* socket: create the socket */
     sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (sockfd < 0) 
         error("ERROR opening socket");
 
     /* gethostbyname: get the server's DNS entry */
     server = gethostbyname(hostname);
     if (server == NULL) {
         fprintf(stderr,"ERROR, no such host as %s\n", hostname);
         exit(0);
     }
 
     /* build the server's Internet address */
     bzero((char *) &serveraddr, sizeof(serveraddr));
     serveraddr.sin_family = AF_INET;
     bcopy((char *)server->h_addr, 
       (char *)&serveraddr.sin_addr.s_addr, server->h_length);
     serveraddr.sin_port = htons(PORTNO);
 
     /* get a message from the user */
     bzero(buf, BUFSIZE);
     printf("Please enter msg: ");
     fgets(buf, BUFSIZE, stdin);
 
     //begin roundTrip
     if(gettimeofday(&start,NULL)) {
         printf("time failed\n");
         exit(1);
        }
     /* send the message to the server */
     serverlen = sizeof(serveraddr);
     n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
     if (n < 0) 
       error("ERROR in sendto");
     
     /* print the server's reply */
     n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
     if (n < 0) 
       error("ERROR in recvfrom");
     else 
     {
         if(gettimeofday(&end,NULL)) 
         {
             printf("time failed\n");
             exit(1);
         }
     }
 
     t1+=start.tv_sec+(start.tv_usec/1000000.0);
     t2+=end.tv_sec+(end.tv_usec/1000000.0);
 
     printf("Echo from server: %s", buf);
     printf("RTT = %g ms\n",(t2-t1)/100);
     return 0;
 }