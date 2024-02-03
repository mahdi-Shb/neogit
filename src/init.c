#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>

int Cinit(int argc,const char* argv[]){
    if (neogitpath){
        printf("You have already initialized the neogit");
        return 1;
    }
    if (mkdir(".\\.neogit")){
        printf("Failed to create directory for neogit\n");
        return 1;
    }
    boolean success=TRUE;
    //making files and dirs inside neogit:
    FILE* file;
    success=success && !mkdir(".\\.neogit\\configs");
    success=success && !mkdir(".\\.neogit\\commits");
    success=success && !mkdir(".\\.neogit\\commits\\specs");
    success=success && !mkdir(".\\.neogit\\stage");
    file=fopen(".\\.neogit\\configs\\alias","w");
    success=success && file;
    file=fopen(".\\.neogit\\commits\\specs\\numberofcommits","w");
    success=success && file;
    if (file){
        fprintf(file,"0");
    }
    //file=fopen(".\\.neogit\\stage","w");
    //success=success && file;
    
    file=fopen(".\\.neogit\\configs\\user","w");
    success=success && file;
    if (!success){
        rmdir(".\\.neogit");
        printf("Failed to initialize neogit");
        return 1;
    }
    fprintf(file,"%s\n%s",defaultname,defaultgmail);
    fclose(file);
    printf("Initialized successfully\n");
    return 0;
}
