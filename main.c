#include <stdio.h>

#include "skl_server.h"




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

