#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <windows.h>
// #include <dirent.h>
// #include <unistd.h>
// #include <stdbool.h>
#include "header.h"
#include "commands.h"
#include "global_functions.h"
int main(int argc, char const *argvv[]){

    chdir("D:\\testing project");
    char a[]="add";
    char b[]=".";
    argc=3;
    char const *argv[]={argvv[0],a,b};

    if (GlobalInit()){
        return 1;
    }
    if (argc<2){
        printf("khob alan man chi ro ejra konam barat? :|");
        return 1;
    }
    for (int i=0; i<NUMBER_OF_COMMANDS; i++){
        if (!strcmp(argv[1],COMMAND_NAMES[i])){
            return COMMAND_FUNCTIONS[i](argc-2,argv+2);
        }
    }
    printf("chert voroodi dadi dadash");
    return 1;
}