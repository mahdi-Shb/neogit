#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
int Cstatus(int argc,const char* argv[]){
    if (argc != 0) {
        printf("status command doesn't need arguments");
        return 1;
    }
    DIR* dir = opendir(".");
    struct dirent* entry;
    readdir(dir);
    readdir(dir);
    while ((entry = readdir(dir)) != NULL){
        
    }
    return 0;
}
