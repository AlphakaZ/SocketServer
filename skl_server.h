#ifndef SKL_SERVER_H
#define SKL_SERVER_H

#include <arpa/inet.h>

#ifndef QUEUELIMIT
#define QUEUELIMIT 5
#endif

#ifndef MSGSIZE
#define MSGSIZE 1024
#endif

#ifndef BUFSIZE
#define BUFSIZE (MSGSIZE + 1)
#endif

typedef struct
{
    // 各ソケットのディスクリプタ
    int serverSocket;
    int clientSocket;

    // 各ソケットのアドレス
    struct sockaddr_in serverSocketAddress;
    struct sockaddr_in clientSocketAddress;

    unsigned short serverPort;
}ServerSocketModule;

// 文字列をポート番号に変換するユーティリティ関数。
// 変換できなければ-1を返す。
int str2portNumber(const char* str);

bool setupServer(int portNumber,ServerSocketModule* sMdl);
bool startServer(ServerSocketModule* sMdl);

//データを渡す、という単位で細かくできたらよい。


#endif // SKL_SERVER_H

