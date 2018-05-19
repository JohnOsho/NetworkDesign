#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define M_MAX 1024 /*max text line length*/
#define SERV_PORT 10010 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

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

int main (int argc, char **argv)
    {
        //struct Message *ptr = (struct Message*) calloc(3, sizeof(*ptr));
        struct Message M2 = {0};
        struct returnMessage returnM = {0};
        char buffer2[sizeof(M2)];
        char returnBuffer[sizeof(returnM)];
        char isValid = '1';
        int listenfd, connfd, n, ans;
        socklen_t clilen;
        //char buf[MAXLINE]; //buffer for data send and recv
        struct sockaddr_in cliaddr, servaddr;//server and client socket addresses
        //creation of the socket
        listenfd = socket (AF_INET, SOCK_STREAM, 0);//preparation of the socket address 
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //kernel chooses source ip address(any interface of the host)
        servaddr.sin_port = htons(SERV_PORT); //host to network byte order conversion
        bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
        listen(listenfd, LISTENQ);
        printf("%s\n","Server running...waiting for connections.");
        for( ; ; ) 
        {
            printf("in the seerver\n");
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);//get the new socket
            printf("%s\n","Received request...");
            while( (n = recv(connfd, buffer2, sizeof(buffer2),0)) > 0)  
            { 
                //n = recv(connfd, &M, M_MAX,0);
                //start receiving on the data sock
                memcpy(&M2, buffer2, sizeof(M2));
                printf("%s","String received from and resent to the client:");
                //puts(buf);
                printf("print elements of array: \n");
                printf("a = %u\n",M2.a);
                printf("b = %u\n",M2.b);
                printf("c = %c\n",M2.c);
                if (M2.c == '+')
                {
                    ans = M2.a + M2.b;
                }
                else if (M2.c == '-')
                {
                    ans = M2.a - M2.b;
                } else if (M2.c == '*')
                {
                    ans = M2.a * M2.b;
                } else if (M2.c == '/')
                {
                    if(M2.b == 0)
                    {
                        ans = 0;
                        isValid = '2';
                    }
                    else 
                    {
                        ans = M2.a / M2.b;
                    }
                    
                }
                //ans = M2.a M2.c M2.b;
                printf("ans = %u\n",ans);
                //printf("sizeof(M) == %d\n", sizeof(*ptr));
                //send(connfd, &ptr, n, 0); //echo it back to the client
                returnM.c = M2.c;
                returnM.a = M2.a;
                returnM.b = M2.b;
                returnM.result = ans;
                returnM.isAnswerValid = isValid;

                memcpy(returnBuffer, &returnM, sizeof(returnM));
                send(connfd, &returnBuffer, sizeof(returnBuffer), 0); // send input to server
            }
            if(n < 0) 
            {
                perror("Read error"); 
                exit(1);
            }
            close(connfd); 
            //finish communication with one client; go back and wait for next client
        }
        //close listening socket
        close(listenfd); 
    }