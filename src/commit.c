#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
void initcommitdata(struct Commitdata* commitdata,char *msg){
    char id[ID_LEN+1];
    get_ID(id);
    strcpy(Commitdata->id,id);
    strcpy(Commitdata->msg,msg);
    char path[_MAX_PATH];
    sprintf(path,"%s\\configs\\user",neogitpath);
    FILE*file =fopen(path,"r");
    fscanf(file,"%s",&Commitdata->username);
    fclose(file);
    sprintf(path,"%s\\lastcommit\\commitdata",neogitpath);
    FILE*file =fopen(path,"r");
    fscanf(file,"%s",&Commitdata->previous_id);
    fscanf(file,"%s",&Commitdata->branch);
    fclose(file);
}
    
boolean find_commit_file(char *findpath,const char* subneogitpath,const char* path, int i){
    while (i--){
        char Subneogitpath[100];
        // char Path[_MAX_PATH];
        sprintf(Subneogitpath,"%s\\%d",subneogitpath,i);
        getneogitpath(findpath,path,Subneogitpath);
        if (fileexist(findpath)){
            return true;
        }
        strcat(findpath,".delneogit");
        if (fileexist(findpath)){
            // *findpath='\0';
            return false;
        }
    }
    // *findpath='\0';
    return false;
}
int copy_commit_file(char *newpath,char *path,char *subneogitcommitpath,int I){
    int fch=0;
    char folderpath[_MAX_PATH];
    for (int i=0;i<I;i++){
        char Subneogitcommitpath[100];
        // char Path[_MAX_PATH];
        sprintf(Subneogitcommitpath,"%s\\%d",subneogitcommitpath,i);
        getneogitpath(folderpath,path,Subneogitcommitpath);
        fch+=ocopyfolder(folderpath,newpath);
    }
}

int Ccommit(int argc,const char* argv[]){
    if (argc != 2) {
        printf("Invalid Command");
        return 1;
    }
    char msg[73];
    if (!strcmp(argv[0],"-s")){
        FILE*file=findshortcut(argv[1]);
        // if (oopen(file)){
        //     return 1;
        // }
        if (feof(file)){
            printf("The shortcut doesn't exist");
            return 1;
        }
        char shortcut[30];
        struct Shmsg shmsg;
        fread(&shmsg,sizeof(struct Shmsg),1,file);
        strcpy(msg,shmsg.msg);
        fclose(file);
    } else if (!strcmp(argv[0],"-m")){
        if (strlen(argv[1]) > MAX_COMMIT_MESSAGE){
            printf("Your commit message is too long\nMaximum allowed characters are %d",MAX_COMMIT_MESSAGE);
            return 1;
        }
        strcpy(msg,argv[0]);
    } else {
        printf("The first argument must be -m or -s");
        return 1;
    }
    char id[ID_LEN+1];
    get_ID(id);
    char path[MAX_PATH];
    struct commitdata;
    initcommitdata(&commitdata);
    sprintf(path,"%s\\commits\\%d",neogitpath,id);
    if (mkdir(path)){
        printf("error creating commit");
        return 1;
    }

}
