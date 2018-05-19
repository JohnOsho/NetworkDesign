#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define M_MAX 1024 /* max Message size*/
#define SERV_PORT 3000 /*port*/

struct __attribute__((__packed__)) Message  {
    unsigned int a; 
    unsigned int b;
    char c;
    
}; // *ptr;

struct __attribute__((__packed__)) returnMessage  {
    char c;
    unsigned int a; 
    unsigned int b;
    unsigned int result;
    char isAnswerValid;
};

int main(int argc, char *argv[])
{
    unsigned int a; 
    unsigned int b;
    char c;
    //char *M;
    //unsigned long int a, b;
    //char c;  // must be '+','-','X','/'
    struct Message M = {0};
    struct returnMessage returnM2 = {0};
    char returnBuffer2[sizeof(returnM2)];
    int sockfd, n;
    struct sockaddr_in servaddr;
    //char sendline[MAXLINE], recvline[MAXLINE];
    //struct Message *ptr = (struct Message*) calloc(3, sizeof(*ptr));
    
    
    // argurment check
    if (argc != 2) 
    {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }
    
    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) 
    {
        perror("Problem in creating the socket");
        exit(2);
    }
    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]); //convert number_dot string to binary
    servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket 
    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) 
    {
        perror("Problem in connecting to the server");
        exit(3);
    }

    printf("Enter a: ");
    scanf("%u",&M.a );
    printf("a = %u\n",M.a);
    printf("Enter b: ");
    scanf("%u",&M.b);
    printf("Enter c (+,-,/,x): ");
    scanf("%s",&M.c);

    char buffer[sizeof(M)]; //=  calloc(3, 9);
    
    //void *buffer=  calloc(3, 9);
    memcpy(buffer, &M, sizeof(M));
    //memcpy(&M2, buffer, sizeof(M));
    //memcpy(buffer + sizeof(unsigned int), &b, sizeof(unsigned int));
    //memcpy(buffer + sizeof(unsigned int) + sizeof(unsigned int), &c, sizeof(char));
    //memcpy(buffer, &a, sizeof(unsigned int));
    //printf("M2.a = %u\n",M2.a);
    //printf("sizeof(a) == %d\n", sizeof(buffer+0));
    //printf("b = %u\n",(buffer[4]));
    //printf("c = %c\n",(buffer[8]));
    //printf("sizeof(M) == %d\n", sizeof(buffer));

    send(sockfd, &buffer, sizeof(buffer), 0); // send input to server
    printf("%s", "message sent: ");
    //free(ptr);

    while( (n = recv(sockfd, returnBuffer2, sizeof(returnBuffer2),0)) > 0)  
    {
        memcpy(&returnM2, returnBuffer2, sizeof(returnM2));
        printf("%s", "String received from the server: \n");
        //printf("a = %u\n",returnM2.a);
        //printf("b = %u\n",returnM2.b);
        //printf("c = %c\n",returnM2.c);
        //printf("resulr = %u\n",returnM2.result);
        //printf("isValid = %c\n",returnM2.isAnswerValid);
        if (returnM2.isAnswerValid == '2')
        {
            printf("NaN result \n");
        }
        else 
        {
            printf("a = %u",returnM2.a);
            printf("b = %u\n",returnM2.b);
            printf("c = %c\n",returnM2.c);
            printf("result = %u\n",returnM2.result);
            
        }
        
    }
    if(n < 0) 
    {
        perror("Read error"); 
        exit(1);
    }
    close(sockfd);
    exit(0);
}
