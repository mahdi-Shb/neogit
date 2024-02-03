#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
int Cconfig(int argc,const char* argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    if (argc==3){
        if (strcmp(argv[0],"-global")){
            printf("Invalid Command\n");
            return 1;
        }
        // ...----------------------
    } else if (argc==2){
        if (strncmp(argv[0],"user.",5)){
            char config_path[_MAX_PATH];
            sprintf(config_path,"%s\\configs\\user",neogitpath);
            FILE* config_file = fopen(config_path,"r");
            if(!config_file) {
                printf("Error opening configs");
                return 1;
            }
            char name[100];
            char gmail[100];
            fscanf(config_file,"%s%s",name,gmail);
            fclose(config_file);
            config_file = fopen(config_path,"w");
            if(!config_file) {
                printf("Error writing configs");
                return 1;
            }

            if (strncmp(argv[0]+5,"gmail",5)){
                fprintf(config_file,"%s\n%s",name,argv[1]);
            } else if (strncmp(argv[0]+5,"name",5)){
                fprintf(config_file,"%s\n%s",argv[1],gmail);
            } else {
                printf("Invalid config key\n");
                return 1;
            }
        } else if (strncmp(argv[0],"alias.",6)){
            char config_path[_MAX_PATH];
            sprintf(config_path,"%s\\configs\\alias",neogitpath);
            FILE* config_file = fopen(config_path,"r+");
            if(!config_file) {
                printf("Error opening configs");
                return 1;
            }
            int i=0;
            char al[51],cmd[200];
            while(!feof(config_file)){
                fscanf(config_file,"%s %s",al,cmd);
                if (!strcmp(al,argv[0]+6)){
                    fseek(config_file,i*250,SEEK_SET);
                    break;
                }
                i++;
                fseek(config_file,i*250,SEEK_SET);
            }
            fprintf(config_file,"%50s%199s\n",argv[0]+6,argv[1]);
        } else {
            printf("Invalid config key\n");
            return 1;
        }
    } else {
        printf("Invalid Command");
        return 1;
    }
    
}