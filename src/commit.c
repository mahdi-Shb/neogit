#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>

int Ccommit(int argc,const char* argv[]){
    if (argc != 2) {
        printf("Invalid Command");
        return 1;
    }
    if (strcmp(argv[0],"-m")){
        printf("The first argument must be -m");
        return 1;
    }
    //Checking the message length
    if (strlen(argv[1]) > MAX_COMMIT_MESSAGE){
        printf("Your commit message is too long\nMaximum allowed characters are %d",MAX_COMMIT_MESSAGE);
        return 1;
    }

    char id[ID_LEN+1];
    get_ID(id);
    char path[MAX_PATH];
    
    sprintf(path,"%s\\commits\\1234567890\\%s",neogitpath,id);
    if (mkdir(path)){
        printf("error creating commit");
        return 1;
    }
}
