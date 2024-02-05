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
    struct stat st;
    char path[_MAX_PATH];
    getcwd(path,_MAX_PATH);
    char* lpath=path+strlen(path);
    DIR* dir = opendir(path);
    struct dirent* entry;
    readdir(dir);
    readdir(dir);
    
    while ((entry = readdir(dir)) != NULL){
        sprintf(lpath,"\\%s",entry->d_name);
        stat(path,&st);
        if (!S_ISDIR(st.st_mode)){
            printf("%s",entry->d_name);
            printf(" %c",IsStage(path) ? '+' : '-');
            printf("%c",getY(path,"\\lastcommit"));
        }
        *lpath='\0';
    }
    return 0;
}
