#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <windows.h>
// #include <dirent.h>
// #include <unistd.h>
// #include <stdbool.h>
#include "commands.h"
#include "global_functions.h"
void may_the_c_surprise_us(){

}
int main(int argc, const char *argv[]){
    may_the_c_surprise_us();

    chdir("D:\\testing project");
    argc=2;
    char const *argvkv[]={"neogit", "branch"};

    if (GlobalInit()){
        return 1;
    }
    if (argc<2){
        printf("khob alan man chi ro ejra konam barat? :|");
        return 1;
    }
    for (int i=0; i<NUMBER_OF_COMMANDS; i++){
        if (!strcmp(argvkv[1],COMMAND_NAMES[i])){
            return COMMAND_FUNCTIONS[i](argc-2,argvkv+2);
        }
    }
    if (argc!=2 || neogitpath==NULL){
        printf("chert voroodi dadi dadash");
    }

    FILE* file=findalias(argv[1]);
    if (!file){
        printf("Can't oprn the alias file");
        return 1;
    }
    if (feof(file)){
        printf("chert voroodi dadi dadash");
        return 1;
    }
    struct Alicmd alicmd;
    fread(&alicmd,sizeof(alicmd),1,file);
    printf("the command is: %s\n",alicmd.cmd);
    fclose(file);
    char *argvv[10];
    argc=1;
    for (char* tok=strtok(alicmd.cmd," ");tok;tok=strtok(NULL," ")){
        argvv[argc++]=tok;
    }
    for (int i=0; i<NUMBER_OF_COMMANDS; i++){
        if (!strcmp(argvv[1],COMMAND_NAMES[i])){
            return COMMAND_FUNCTIONS[i](argc-2,(const char **)(argvv+2));
        }
    }
    printf("chert voroodi dadi dadash");
    return 1;
}