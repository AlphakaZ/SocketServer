#include "skl_server.h"

#include <stdio.h> //printf(), fprintf(), perror()

#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <sys/fcntl.h>
#include <sys/types.h>

#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()

#include <string.h> //memset()
#include <unistd.h> //close()

#include <netinet/in.h>
#include <netdb.h>

#include <stdbool.h>

int str2portNumber(const char *str)
{
    int n = (unsigned short) atoi(str);
        if(n == 0){
            fprintf(stderr, "invalid port number.\n");
            return -1;
        }
    return n;
}

bool setupServer(int portNumber, ServerSocketModule *sMdl)
{

}

bool startServer(ServerSocketModule *sMdl)
{

}
