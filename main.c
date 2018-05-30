#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "skl_server.h"



// from Nakano
int parse_url(const char *request, char* url){

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

const char header200[] =  "HTTP/1.0 200 OK\r\n"
                                "Content-Type: %s\r\n"
                                "Transfer-Encoding: chunked\r\n"
                                "\r\n";

const char header404[] = "HTTP/1.0 404 Not Found\r\n"
                         "Status: 404\r\n"
                         "Content-Type: text/html\r\n"
                         "Transfer-Encoding: chunked\r\n"
                         "\r\n"
                         "<h1>404 Error</h1>";

#define ENCODING chunked;

//レスポンスヘッダの生成関数

const char* generateHttpStatusHeader(int code){
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

const char* getCurrentDirectory(){
    static char currentdir[BUFSIZE];
    memset(currentdir, '\0', BUFSIZE);
    return getcwd(currentdir,BUFSIZE);
}

typedef enum{
    EXT_HTML,
    EXT_JS,
    EXT_PHP,
    EXT_SIZE
}EXTENDIONS;
const char* extensions[]={
    "html"
    "js"
    "php"
};

int getExtension(const char* filePath){
    const char *ext = strrchr(filePath, '.');
    ext++;
    for(int i=0;i<EXT_SIZE;++i){
        if(strstr(ext,extensions[i])==0){
            return i;
        }
    }
    return -1;//存在しない拡張子
}


int httpServer(ServerSocketModule* sMdl,ClientSocketModule* cMdl)
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


int main(int argc, char** argv)
{
    
    ServerSocketModule sMdl;

    int port;
    if(argc==2 && (port = str2portNumber(argv[1])) != -1){
        setupServer(port,&sMdl);
        startServerLoop(&sMdl,httpServer);
    }else{
        //引数が不正
    }
    return 0;
}

