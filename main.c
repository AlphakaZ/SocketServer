

#include "skl_webserver.h"
#include "skl_serverutil.h"

int main(int argc, char** argv)
{
    int port;
    if(argc==2 && (port = str2portNumber(argv[1])) != -1){
        createWebServer(port);//loop
    }else{

    }
    return 0;
}

