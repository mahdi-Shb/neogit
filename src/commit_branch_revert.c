#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
void initcommitdata(struct Commitdata* commitdata,char *msg){
    get_ID(commitdata->id);
    strcpy(commitdata->msg,msg);
    char path[_MAX_PATH];
    sprintf(path,"%s\\configs\\user",neogitpath);
    FILE*file =fopen(path,"r");
    fscanf(file,"%s",commitdata->username);
    fclose(file);
    sprintf(path,"%s\\lastcommit\\commitdata",neogitpath);
    file =fopen(path,"r");
    if (file){
        struct Commitdata cd;
        fread(&cd,sizeof(cd),1,file);
        strcpy(commitdata->previous_id,cd.id);
        strcpy(commitdata->branch,cd.branch);
        fclose(file);
    } else{
        *commitdata->previous_id='\0';
        strcpy(commitdata->branch,DEFAULT_BRANCH);
    }
    commitdata->T=time(NULL);
}
void ith_commit(char*id,int I){
    int i=0;
    char path[MAX_PATH];
    sprintf(path,"%s\\lastcommit\\commitdata",neogitpath);
    FILE* file=fopen(path,"rb");
    strcpy(id,"erewf");
    while(*id && I--){
        struct Commitdata commitdata;
        fread(&commitdata,sizeof(commitdata),1,file);
        fclose(file);
        strcpy(id,commitdata.previous_id);
        // if (!(*id)){
        //     return;
        // }
        sprintf(path,"%s\\commits\\%s\\commitdata",neogitpath,id);
        file=fopen(path,"rb");
    }
    fclose(file);
}
boolean revert_files(char *newpath,char* lastcommitid){
    char id[numberofcommits][ID_LEN];
    char path[_MAX_PATH];
    int i=0;
    strcpy(id[0],lastcommitid);
    sprintf(path,"%s\\commits\\%s\\commitdata",neogitpath,lastcommitid);
    FILE* file=fopen(path,"rb");
    if (!file){
        return false;
    }
    fclose(file);
    while(*id[i++]){
        struct Commitdata commitdata;
        sprintf(path,"%s\\commits\\%s\\commitdata",neogitpath,id[i-1]);
        file=fopen(path,"rb");
        fread(&commitdata,sizeof(commitdata),1,file);
        fclose(file);
        strcpy(id[i],commitdata.previous_id);
    }
    i--;
    while (i--){
        char folderpath[_MAX_PATH];
        sprintf(folderpath,"%s\\commits\\%s",neogitpath,id[i]);
        ocopyfolder(folderpath,newpath);
    }
    return true;
}

int Ccommit(int argc,const char* argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    if (argc != 2) {
        printf("Invalid Command");
        return 1;
    }
    char msg[73];
    if (!strcmp(argv[0],"-s")){
        FILE*file=findshortcut(argv[1]);
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
        strcpy(msg,argv[1]);
    } else {
        printf("The first argument must be -m or -s");
        return 1;
    }
    char path[MAX_PATH];
    sprintf(path,"%s\\stage",neogitpath);
    if (isempty(path)){
        printf("There is no file in stage");
        return 1;
    }
    struct Commitdata commitdata;
    initcommitdata(&commitdata,msg);
    char Path[MAX_PATH];
    sprintf(Path,"%s\\commits\\%s",neogitpath,commitdata.id);
    if (mkdir(Path)){
        printf("error creating commit");
        return 1;
    }
    commitdata.fileschanged=normalcopyfolder(path,Path);

    // lastcommit and laststage
    sprintf(path,"%s\\laststage",neogitpath);
    deleteinsidefolder(path);
    sprintf(path,"%s\\stage",neogitpath);
    deleteinsidefolder(path);
    // writing commitdata
    strcat(Path,"\\commitdata");
    FILE* file=fopen(Path,"wb");
    if (!file){
        printf("Can't open file");
        return 1;
    }
    fwrite(&commitdata,sizeof(struct Commitdata),1,file);
    fclose(file);

    Path[strlen(Path)-strlen("\\commitdata")]='\0';
    sprintf(path,"%s\\lastcommit",neogitpath);
    ocopyfolder(Path,path);
    sprintf(Path,"%s\\commitdatas",neogitpath);
    file=fopen(Path,"ab");
    if (!file){
        printf("Can't open commitdatas file");
        return 1;
    }
    fwrite(&commitdata,sizeof(commitdata),1,file);
    fclose(file);
}
int findlastbranchname(const char *branchname){
    char path[_MAX_PATH];
    sprintf(path,"%s\\commitdatas",neogitpath);
    FILE* file=fopen(path,"rb");
    struct Commitdata cd;    
    int I=-1;
    for (int i=0;fread(&cd,sizeof(cd),1,file);i++){
        if (!strcmp(branchname,cd.branch)){
            I=i;
        }
    }
    return I;
}
int Cbranch(int argc, const char *argv[]){
    if (argc == 1){
        if (findlastbranchname(argv[0])!=-1){
            printf("The branch has already created");
            return 1;
        }
        //making commitdata
        struct Commitdata commitdata;
        char msg[73];
        sprintf(msg,"branch \"%s\" created",argv[0]);
        initcommitdata(&commitdata,msg);
        strcpy(commitdata.branch,argv[0]);
        commitdata.fileschanged=0;

        //creating new commit
        char Path[MAX_PATH];
        sprintf(Path,"%s\\commits\\%s",neogitpath,commitdata.id);
        if (mkdir(Path)){
            printf("error creating commit");
            return 1;
        }
        strcat(Path,"\\commitdata");
        FILE* file=fopen(Path,"wb");
        if (!file){
            printf("Can't open file");
            return 1;
        }
        fwrite(&commitdata,sizeof(struct Commitdata),1,file);
        fclose(file);

        //writing on lastcommitcommitdata
        sprintf(Path,"%s\\lastcommit\\commitdata",neogitpath);
        file=fopen(Path,"wb");
        if (!file){
            printf("Can't open last commit file");
            return 1;
        }
        fwrite(&commitdata,sizeof(commitdata),1,file);
        fclose(file);

        //writing on commitdatas
        sprintf(Path,"%s\\commitdatas",neogitpath);
        file=fopen(Path,"ab");
        if (!file){
            printf("Can't open commitdatas file");
            return 1;
        }
        fwrite(&commitdata,sizeof(commitdata),1,file);
        fclose(file);

        printf("branch successfully created");
        return 0;
    } else if (argc==0){
        return printbranchs();
    } else {
        printf("Invalid");
        return 1;
    }
}
int Crevert(int argc, const char *argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    if (argc==0){
        printf("Invalid argument numbers");
        return 1;
    }
    char id[ID_LEN+1]="";
    int argcc=0;
    int I;
    boolean n=false;
    if (!strcmp(argv[0],"-n")){
        n=true;
        argc--;
        argv++;
    } else if (sscanf(argv[0],"HEAD-%d",&I)){
        argc--;
        argv++;
        ith_commit(id,I);
        printf(id);
        if (!(*id)){
            printf("nemitoonam inghadar aghab bargardam");
            return 1;
        }
        printf("reverting to commit %s ...\n",id);
        argcc++;
    }
    char msg[73];
    if (!(argc%2)){
        printf("Invalid argument numbers");
        return 1;
    }
    if (!strcmp(argv[argcc],"-e")){
        argcc+=2;
        strcpy(msg,argv[1]);
    }
    if (!strcmp(argv[argcc],"-m")){
        argcc+=2;
        printf("hanooz -m sakhte nashode");
        //strcpy(msg,argv[1]);
    }
    if (!(*id)){
        strcpy(id,argv[argcc]);
    }
    if (!(*msg)){
        sprintf(msg,"commit %s reverted",id);
    }

    char aPath[_MAX_PATH],lcPath[MAX_PATH],cPath[MAX_PATH];
    strcpy(aPath,neogitpath);
    aPath[strlen(neogitpath)-strlen("\\.neogit")]='\0';
    sprintf(lcPath,"%s\\lastcommit",neogitpath);

    // sprintf(cPath,"%s\\commits\\%s",neogitpath,id);
    // deleteinsidefolder(lcPath);
    // normalcopyfolder(cPath,lcPath);
    struct Commitdata commitdata;
    initcommitdata(&commitdata,msg);
    
    if (!n){
        sprintf(cPath,"%s\\commits\\%s",neogitpath,commitdata.id);
        if (mkdir(cPath)){
            printf("error creating commit");
            return 1;
        }
    }
    
    deleteinsidefolder(aPath);
    if (!revert_files(aPath,id)){
        printf("id %s not found",id);
        return 1;
    }
    if (!n){
        strcat(aPath,"\\commitdata");
        remove(aPath);
        aPath[strlen(neogitpath)-strlen("\\.neogit")]='\0';
        ncopyfolder(aPath,cPath,lcPath);
    }

    deleteinsidefolder(lcPath);
    commitdata.fileschanged = normalcopyfolder(aPath,lcPath);
    if (n){
        strcat(aPath,"\\commitdata");
        remove(aPath);
        aPath[strlen(neogitpath)-strlen("\\.neogit")]='\0';    
    }
    // writing commitdata
    if (!n){
        strcat(lcPath,"\\commitdata");
        FILE* file=fopen(lcPath,"wb");
        if (!file){
            printf("Can't write commitdatas");
            return 1;
        }
        fwrite(&commitdata,sizeof(struct Commitdata),1,file);
        fclose(file);

        strcat(cPath,"\\commitdata");
        file=fopen(cPath,"wb");
        if (!file){
            printf("Can't write commitdatas");
            return 1;
        }
        fwrite(&commitdata,sizeof(struct Commitdata),1,file);
        fclose(file);

        char cdsPath[MAX_PATH];
        sprintf(cdsPath,"%s\\commitdatas",neogitpath);
        file=fopen(cdsPath,"ab");
        if (!file){
            printf("Can't write commitdatas");
            return 1;
        }
        fwrite(&commitdata,sizeof(struct Commitdata),1,file);
        fclose(file);
    }
    printf("successfully reverted");
    if (!n){
        printf("and committed with id %s",commitdata.id);
    } 
}