#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFSIZE 1024
#define PORTNO 10010   /* port to listen on */

/* 
 * error - wrapper for perror
 */
 void error(char *msg) 
 {
    perror(msg);
    exit(0);
}

// function to convert int to string
void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

//main
int main(int argc, char **argv) 
    {
        int sockfd, n;
        int serverlen;
        struct sockaddr_in serveraddr;
        struct hostent *server;
        char *hostname;
        struct timeval start,end;
        double t1,t2;
        int num_packet_sent; // int to be sent
        int counter; // count number of packets sent to server
        char str[10];
        pid_t childpid;
        t1=0.0; t2=0.0;
        counter = 0; 

        /* check command line arguments */
        if (argc != 2) 
        {
            fprintf(stderr,"usage: %s <hostname> \n", argv[0]);
            exit(0);
        }
        hostname = argv[1];

        /* socket: create the socket */
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");

        /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(hostname);
        if (server == NULL) 
        {
            fprintf(stderr,"ERROR, no such host as %s\n", hostname);
            exit(0);
        }

        /* build the server's Internet address */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
	    (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(PORTNO);

        serverlen = sizeof(serveraddr);


        //send 10000
        num_packet_sent = 1;
        printf("Sending 10000 messages 1 byte each and wait for ack.\n");

        
        childpid = fork ();
        for(;num_packet_sent<10000;)
        {
            //convert int to char
            tostring(str, num_packet_sent);
            printf("\n");
            //get strat time
            if(gettimeofday(&start,NULL)) 
            {
                printf("time failed\n");
                exit(1);
            }
            n = sendto(sockfd, str, strlen(str), 0, &serveraddr, serverlen);
            counter = counter + 1;
            //print client ouput
            printf("client sent : %s",str);
            printf("\n");
            if (n < 0) 
              error("ERROR in sendto");
            //call child process
            
            
            
    
           // printf("\nRTT = %g ms\n",(t2-t1)/100);
            num_packet_sent += 1;
        }
        if (childpid == 0)
        {
            
            
            
            
            while ( n = recvfrom(sockfd, str, strlen(str), 0, &serveraddr, &serverlen) > 0)
            {
                /* print the server's reply */
            printf("\nEcho from server : ");
            fputs(str, stdout);
            printf("\n");
            
            if (n < 0) 
            {
                error("ERROR in recvfrom");
                exit(1);
            }
            //get time client recieved ECHO
            if(gettimeofday(&end,NULL)) 
                {
                    printf("time failed\n");
                    exit(1);
                }
                t1+=start.tv_sec+(start.tv_usec/1000000.0);
                t2+=end.tv_sec+(end.tv_usec/1000000.0);
                printf("RTT = %g ms\n",(t2-t1)/100);
            }
            
        }
        
        //printf("counter: %d\n", counter);
        return 0;
    }

