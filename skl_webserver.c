#include "skl_webserver.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "skl_server.h"
#include "skl_serverutil.h"

// from Nakano
static int parse_url(const char *request, char* url){

    char *start = strstr(request, "GET /");//url開始位置
    char *end = strstr(request, "HTTP/");//url終了位置

    if(start == NULL || end == NULL){
        return false;
    }

    start += 5;
    int i;
    for(i=0; start+i!=end; i++){
        url[i] = start[i];
    }
    url[i] = '\0';
    return true;
}

//もっと細かく分けて生成すべき

static const char header200[] =  "HTTP/1.0 200 OK\r\n"
                                "Content-Type: %s\r\n"
                                "Transfer-Encoding: chunked\r\n"
                                "\r\n";

static const char header404[] = "HTTP/1.0 404 Not Found\r\n"
                         "Status: 404\r\n"
                         "Content-Type: text/html\r\n"
                         "Transfer-Encoding: chunked\r\n"
                         "\r\n"
                         "<h1>404 Error</h1>";

//レスポンスヘッダの生成関数
static const char* generateHttpStatusHeader(int code){
    switch(code){

    case 200: // 正常
        return header200;
    case 301: // ページの引っ越し
    case 302: // ページの一時的な引っ越し
    case 304: // 未更新(前に渡したのと同じ)
    case 403: // forbidden

    case 404:
    default:
        return header404;
    }
}

static int httpServer(ServerSocketModule* sMdl,ClientSocketModule* cMdl)
{
    //httpの通信処理を記述する。接続は終了している。
    char request[BUFSIZE];
    char url[BUFSIZE];

    recvmsgfromclient(cMdl,request,BUFSIZE);
    printf("request:\n%s",request);

    if(!parse_url(request,url)){
        // リクエストが不正
        return 0;
    }

    if(getExtension(url) == -1){
        sendmsg2client(cMdl,generateHttpStatusHeader(404));
        return 404;
    }

    char filepath[BUFSIZE];

    snprintf(filepath, BUFSIZE, "%s/%s", getCurrentDirectory(), url);
    // ファイルの存在チェック

    if(!sendfile2client(cMdl,filepath)){
        sendmsg2client(cMdl,generateHttpStatusHeader(404));
        return 404;
    }else{
        return 0;
    }

}

void createWebServer(int port){
    ServerSocketModule sMdl;
    setupServer(port,&sMdl);
    startServerLoop(&sMdl,httpServer);
}