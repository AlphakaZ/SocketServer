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

static int generateSocket()
{
    int n = socket(SKL_SOCKET_DOMAIN_IPV4,
                   SKL_SOCKET_TYPE_TCP,
                   SKL_SOCKET_PROTOCOL_TCP);
    if(n < 0){
            perror("socket() failed.");
            return -1;
    }
    return n;
}

static bool initServerSocketAddress(ServerSocketModule* sMdl){

    struct sockaddr_in* addr = &(sMdl->serverSocketAddress);

    memset(addr,0,sizeof(*addr));
    addr->sin_family      = SKL_SOCKET_ADDR_IPV4;//アドレスファミリ
    addr->sin_addr.s_addr = htonl(INADDR_ANY);//0.0.0.0のアドレス
    addr->sin_port        = htons(sMdl->serverPort);
    //ポート番号の格納 htonl(),htons()はネットワークに対応するエンディアン変換関数

    // アドレスとポート番号を設定する。
    // 特に通信で用いるアドレスはビッグエンディアンであるため、この関数を挟む必要がある。
    if (bind(sMdl->serverSocket,(struct sockaddr *) addr, sizeof(*addr) ) < 0 ) {
        perror("bind() failed.");
        return false;
    }
    return true;
}

int str2portNumber(const char *str)
{
    int n = (unsigned short) atoi(str);
        if(n == 0){
            fprintf(stderr, "invalid port number.\n");
            return -1;
        }
    return n;
}

static bool readyServer(ServerSocketModule* sMdl){
    if (listen(sMdl->serverSocket, QUEUELIMIT) < 0) {
        perror("listen() failed.");
        return false;
    }
    return true;
}

bool setupServer(int portNumber, ServerSocketModule *sMdl)
{
    sMdl->serverPort = portNumber;//ポート番号を設定
    int sock = generateSocket();
    if(sock == -1){
        return false;
    }
    sMdl->serverSocket = sock;//ソケットを設定

    if(!initServerSocketAddress(sMdl))//ソケット構造体のアドレス周辺を初期化
    {
        return false;
    }

    // listen状態にする。
    if(!readyServer(sMdl)){
        return false;
    }
    return true;
}

bool startServer(ServerSocketModule *sMdl,SERVER_RESPONSE_FUNC func)
{
    unsigned int clientLen; // client internet socket address length

    while(1) {
        clientLen = sizeof(sMdl->clientSocketAddress);
        if ((sMdl->clientSocket = accept(sMdl->serverSocket, (struct sockaddr *) &(sMdl->clientSocketAddress), &clientLen)) < 0) {
            perror("accept() failed.");
            return false;
        }else{
            printf("connected from %s.\n", inet_ntoa(sMdl->clitSockAddr.sin_addr));
            func(sMdl);
        }
    }
}

void closeServer(ServerSocketModule *sMdl)
{
    close(sMdl->serverSocket);
}
