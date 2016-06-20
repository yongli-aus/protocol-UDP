#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv)
{
    struct sockaddr_in server_add;
    int s, i, slen=sizeof(server_add);
    char buf[BUFLEN];
    char message[BUFLEN];
    if (argc !=2) {
         perror("Usage: TCPClient <IP address of the server");
         exit(1);
        }
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &server_add, 0, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(PORT);
     server_add.sin_addr.s_addr=inet_addr(argv[1]);


    while(1)
    {
        printf("Enter message : ");
        gets(message);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &server_add, slen)==-1)
        {
            die("sendto()");
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        memset(message,'\0',BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &server_add, &slen) == -1)
        {
            die("recvfrom()");
        }

        puts(buf);
    }

    close(s);
    return 0;
}
