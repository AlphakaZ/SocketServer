#ifndef SKL_SERVER_UTIL_H
#define SKL_SERVER_UTIL_H

#define DIRECTORY_BUF_SIZE 256

typedef enum{
    EXT_HTML,
    EXT_JS,
    EXT_PHP,
    EXT_SIZE
}EXTENDIONS;

static const char* extensions[]={
    "html"
    "js"
    "php"
};

// 文字列をポート番号に変換するユーティリティ関数。
// 変換できなければ-1を返す。
int str2portNumber(const char* str);
const char* getCurrentDirectory();

int getExtension(const char* filePath);

#endif