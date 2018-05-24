#include <stdio.h>

#include "skl_server.h"

typedef enum perror{
    NO_ERROR = 0,
    NO_GET = 1 << 1,
    NO_HTTP = 1 << 2,
}parseerror;

//
int parse_url(const char *request, char* url){
    char *start = strstr(request, "GET /");
    char *end = strstr(request, "HTTP/");

    int ret=NO_ERROR;
    if(start == NULL){
        ret |= NO_GET;
    }
    if(end == NULL){
        ret |= NO_HTTP;
    }

    if(ret != NO_ERROR){
        return ret;
    }

    start += 5;
    for(int i=0; start+i!=end; i++){
        url[i] = start[i];
    }
    url[i] = '\0';
    return ret;
}

int httpServer(ServerSocketModule* sMdl,ClientSocketModule* cMdl)
{
    //httpの通信処理を記述する。接続は終了している。
    return 1;
}


int main(int argc, char** argv)
{
    
    ServerSocketModule sMdl;

    int port;
    if((port = str2portNumber(argv[0])) != -1){
        setupServer(port,&sMdl);
        startServer(&sMdl,httpServer);
    }
    return 0;
}

