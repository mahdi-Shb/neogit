#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

boolean find_commit_file(char *findpath,const char* path, char * id){
    char subneogitpath[MAX_PATH];
    char cdPath[MAX_PATH];
    sprintf(subneogitpath,"commits\\%s",id);
    sprintf(cdPath,"%s\\lastcommit\\commitdata",neogitpath);
    FILE* file=fopen(path,"rb");
    if (!file){
        printf("There isn't any commit id %s",id);
        return 1;
    }
    while(*id){
        struct Commitdata commitdata;
        fread(&commitdata,sizeof(commitdata),1,file);
        fclose(file);
        strcpy(id,commitdata.previous_id);

        sprintf(cdPath,"%s\\commits\\%s\\commitdata",neogitpath,id);
        getneogitpath(findpath,path,subneogitpath);
        if (fileexist(findpath)){
            fclose(file);
            return !isdeletefile(findpath);
        }
    
        sprintf(subneogitpath,"commits\\%s",id);
        file=fopen(cdPath,"rb");
    }
    fclose(file);
    return false;
}

boolean hasthewordwc(const char*s,const char*word){
    char S[strlen(s)+1];
    strcpy(S,s);
    for (char* token = strtok(S," ?!.,()"); token ;token = strtok(NULL," ?!.,()")){
        if (match_wildcard(token,word)){
            return true;
        }
    }
    return false;
}

int Cgrep(int argc,const char* argv[]){
    const char** argvv=argv;
    if (argc<4){
        printf("Too few arguments\n");
        return 1;
    }
    if (strcmp(argv[0],"-f")){
        printf("The first argument must be -f");
        return 1;
    }
    char filepath[MAX_PATH];
    GetFullPathNameA(argv[1],MAX_PATH,filepath,NULL);
    if (strcmp(argv[2],"-p")){
        printf("The third argument must be -p");
        return 1;
    }
    boolean n=false;
    char id[ID_LEN+1]="";
    argc-=4;
    argv+=4;
    printf("%d \n%s",argc,argv[0]);
    while (argc){
        if (!strcmp(argv[0],"-n") && !n){
            n=true;
            argc--;
            argv++;
        } else if (strcmp(argv[0],"-c") && !*id){
            strcpy(id,argvv[1]);
            argc-=2;
            argv+=2;
        } else {
            printf("Invalid");
            return 1;
        }
    }
    char findpath[MAX_PATH];
    if (*id){
        if (!find_commit_file(findpath,filepath,id)){
            printf("Can't find the file");
            return 1;
        }
    } else {
        strcpy(findpath,filepath);
    }
    FILE* file=fopen(findpath,"r");
    if (!file){
        printf("Can't find the file");
        return 1;
    }
    char A[400];
    for(int i=0;fgets(A,400,file);i++){
        if (hasthewordwc(A,argvv[3])){
            if (n){
                printf("%d. ",i);
            }
            printf("%s",A);
        }
    }
    return 0;
}
    // char lcdPath[MAX_PATH];
        // sprintf(lcdPath,"%s\\lastcommit\\commitdata",neogitpath);
        // FILE* file=fopen(lcdPath,"rb");
        // if(!file){
        //     printf("Ctag: can't open lastcommit commitdata file");
        //     return 1;
        // }
        // struct Commitdata cd;
        // fread(&cd,sizeof(cd),1,file);
        // fclose(file);
        // strcpy(id,cd.id);
        // printf("id:%s",id);
    