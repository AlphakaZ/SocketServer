#ifndef SKL_SERVER_H
#define SKL_SERVER_H

#include <arpa/inet.h>
#include <stdbool.h>

//これらの数値については上書き可能
#ifndef QUEUELIMIT
#define QUEUELIMIT 5
#endif

#ifndef MSGSIZE
#define MSGSIZE 1024
#endif

#ifndef BUFSIZE
#define BUFSIZE (MSGSIZE + 1)
#endif

// 分かりやすいように、各変数にエイリアスを用意
#define SKL_SOCKET_DOMAIN_IPV4 PF_INET
#define SKL_SOCKET_TYPE_TCP SOCK_STREAM
#define SKL_SOCKET_PROTOCOL_TCP IPPROTO_TCP
#define SKL_SOCKET_ADDR_IPV4 AF_INET

typedef struct
{
    // ソケットのディスクリプタ
    int serverSocket;
    // ソケットのアドレス
    struct sockaddr_in serverSocketAddress;
    unsigned short serverPort;
}ServerSocketModule;

typedef struct
{
    int socket;
    unsigned int addressLength;
    struct sockaddr_in socketAddress;
}ClientSocketModule;

typedef int(*SERVER_RESPONSE_FUNC)(ServerSocketModule* sMdl,ClientSocketModule* cMdl);

// 文字列をポート番号に変換するユーティリティ関数。
// 変換できなければ-1を返す。
int str2portNumber(const char* str);

bool setupServer(int portNumber,ServerSocketModule* sMdl);

// funcを実行し、クライアントとの接続に失敗すると-1を返す。
// それ以外は、funcの数値をそのまま返す。
///
/// \brief funcを一度だけ実行する。
/// \param sMdl 使用したいsetup()済みのサーバモジュール
/// \param func クライアントとの通信に利用したい関数。ServerSocketModuleを引数にもち、返り値はint。
/// \return 接続に失敗：-1, それ以外, funcの戻り値
///
int startServer(ServerSocketModule* sMdl,SERVER_RESPONSE_FUNC func);

///
/// \brief funcをループ実行させる。funcの戻り値が-1の場合のみループを抜けて終了する。
/// \param sMdl 使用したいsetup()済みのサーバモジュール
/// \param func クライアントとの通信に利用したい関数。戻り値が-1であるときループを終了する。
/// \return
///
int startServerLoop(ServerSocketModule* sMdl,SERVER_RESPONSE_FUNC func);

void closeServer(ServerSocketModule* sMdl);

#endif // SKL_SERVER_H

