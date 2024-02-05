#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
FILE* findshortcut(const char* shortcut){
    char path[_MAX_PATH];
    sprintf(path,"%s\\shortcuts",neogitpath);
    FILE *file = fopen(path,"rb+");
    if(!file) {
        printf("Error opening shortcuts");
        return NULL;
    }
    int i=0;
    struct Shmsg shmsg;
    while(fread(&shmsg,sizeof(struct Shmsg),1,file)){
        if (!strcmp(shortcut,shmsg.sh)){
            fseek(file,i*100,SEEK_SET);
            break;
        }
        i++;
        fseek(file,i*100,SEEK_SET);
    }
    return file;
}
int Cset(int argc,const char* argv[]){
    if (argc != 4){
        printf("Error: invalid number of arguments for set command\n");
        return 1;
    }
    if (strcmp("-m",argv[0])){
        printf("The first argument after \"set\" must be \"-m\"");
        return 1;
    }
    if (strcmp("-s",argv[2])){
        printf("The third argument after \"set\" must be \"-s\"");
        return 1;
    }
    if (strlen(argv[1])>72){
        printf("max message length is 72");
        return 1;
    }
    if (strlen(argv[3])>26){
        printf("max shortcut name length is 26");
        return 1;
    }
    FILE* file=findshortcut(argv[3]);
    if (!file){
        return 1;
    }
    if (!feof(file)){
        printf("You had set the shortcut before\nUse neogit replace command if you want to replace it");
        fclose(file);
        return 1;
    }
    struct Shmsg shmsg;
    strcpy(shmsg.msg,argv[1]);
    strcpy(shmsg.sh,argv[3]);
    fwrite(&shmsg,sizeof(struct Shmsg),1,file);
    fclose(file);
} 
int Creplace(int argc,const char* argv[]){
    if (argc != 4){
        printf("Error: invalid number of arguments for set command\n");
        return 1;
    }
    if (strcmp("-m",argv[0])){
        printf("The first argument after \"set\" must be \"-m\"");
        return 1;
    }
    if (strcmp("-s",argv[2])){
        printf("The third argument after \"set\" must be \"-s\"");
        return 1;
    }
    if (strlen(argv[1])>72){
        printf("max message length is 72");
        return 1;
    }
    FILE* file=findshortcut(argv[3]);
    if (!file){
        return 1;
    }
    if (feof(file)){
        printf("You hadn't set the shortcut before\nUse neogit set command if you want to set it");
        fclose(file);
        return 1;
    }
    struct Shmsg shmsg;
    strcpy(shmsg.msg,argv[1]);
    strcpy(shmsg.sh,argv[3]);
    fwrite(&shmsg,sizeof(struct Shmsg),1,file);
    fclose(file);
    return 0;
}
int Cremove(int argc,const char* argv[]){
    if (argc != 2){
        printf("Error: invalid number of arguments for set command\n");
        return 1;
    }
    if (strcmp("-s",argv[0])){
        printf("The first argument after \"set\" must be \"-s\"");
        return 1;
    }
    FILE* file=findshortcut(argv[1]);
    if (!file){
        return 1;
    }
    if (feof(file)){
        printf("The shortcut doesn't exist");
        fclose(file);
        return 1;
    }
    int i=ftell(file)/100;
    fseek(file,0,SEEK_END);
    struct Shmsg R[ftell(file)/100-1];
    fseek(file,0,SEEK_SET);
    fread(R,100,i,file);
    fseek(file,100,SEEK_CUR);
    fread(R+i,100,sizeof(R)/sizeof(struct Shmsg)-i,file);
    fclose(file);
    char path[_MAX_PATH];
    sprintf(path,"%s\\shortcuts",neogitpath);
    file = fopen(path,"wb+");
    fwrite(R,1,sizeof(R),file);
    fclose(file);
}
