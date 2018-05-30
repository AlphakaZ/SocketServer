#include "skl_serverutil.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int str2portNumber(const char *str)
{
    int n = (unsigned short)strtol(str,(char**)NULL,10);
        if(n == 0){
            fprintf(stderr, "invalid port number.\n");
            return -1;
        }
    return n;
}

const char* getCurrentDirectory(){
    static char currentdir[DIRECTORY_BUF_SIZE];
    memset(currentdir, '\0', DIRECTORY_BUF_SIZE);
    return getcwd(currentdir,DIRECTORY_BUF_SIZE);
}

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