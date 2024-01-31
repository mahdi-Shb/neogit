#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
int Cadd(int argc,const char* argv[]){

    return 0;
}
int Cconfig(int argc,const char* argv[]){
    if (!neogitdir){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    wchar_t config_path[_MAX_PATH];
    swprintf(config_path,_MAX_PATH,"%s\\configs",neogitdir);
    FILE* config_file = _wfopen(config_path,"r");
    if(!config_file) {
        printf("Error opening configs");
        return 1;
    }
    char name[_MAX_CONFIG_SIZE];
    char gmail[_MAX_CONFIG_SIZE];
    fscanf("%s%s",name,gmail);
    fclose(config_file);
    if (argc==3){
        if (strcmp(argv[0],"-global")){
            printf("Invalid Command\n");
            return 1;
        }
        // ...
    } else if (argc==2){
        if (strncmp(argv[0],"user.",5)){
            if (strncmp(argv[0]+5,"gmail",5)){
                config_file = _wfopen(config_path);
                if(!config_file) {
                    printf("Error writing configs");
                    return 1;
                }
                fprintf(config_file,"%s%s")
                
            } else if (strncmp(argv[0]+5,"name",5)){
                
                
            } else {
                printf("Invalid config key\n");
            }
        } else if (strncmp(argv[0],"alias.",6)){
            add_command_alias(argv[0]+6,argv[1]);
        } else {

        }

    } else {
        printf("Invalid Command");
        return 1;
    }
    
}
int Cinit(int argc,const char* argv[]){
    if (neogitdir){
        printf("You have already initialized the neogit");
        return 1;
    }
    if (mkdir(".\\neogit")){
        printf("Failed to create directory for neogit\n");
        return 1;
    }
    printf("Initialized successfully\n");
    //making files and dirs inside neogit:
    return 0;
}
int Crest(int argc,const char* argv[]){

}
int Creset(int argc,const char* argv[]){

}
int Cundo(int argc,const char* argv[]){

}
int Cstatus(int argc,const char* argv[]){

}
int Ccommit(int argc,const char* argv[]){

}
int Cset(int argc,const char* argv[]){

}
int Creplace(int argc,const char* argv[]){

}
int Cremove(int argc,const char* argv[]){

}
