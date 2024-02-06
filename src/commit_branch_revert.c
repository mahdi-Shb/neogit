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
        strcpy(commitdata->previous_id,cd.previous_id);
        strcpy(commitdata->branch,cd.branch);
        fclose(file);
    } else{
        *commitdata->previous_id='\0';
        strcpy(commitdata->branch,DEFAULT_BRANCH);
    }
    commitdata->T=time(NULL);
}
// boolean find_commit_file(char *findpath,const char* subneogitpath,const char* path, int i){
//     while (i--){
//         char Subneogitpath[100];
//         sprintf(Subneogitpath,"%s\\%d",subneogitpath,i);
//         getneogitpath(findpath,path,Subneogitpath);
//         if (fileexist(findpath)){
//             return true;
//         }
//         strcat(findpath,".delneogit");
//         if (fileexist(findpath)){
//             return false;
//         }
//     }
//     return false;
// }
void revert_files(char *newpath,char* lastcommitid){
    char id[numberofcommits][ID_LEN];
    int i=0;
    strcpy(id[0],lastcommitid);
    while(id[i++]){
        char path[_MAX_PATH];
        struct Commitdata commitdata;
        sprintf(path,"%s\\commit\\%s\\commitdata",neogitpath,id);
        FILE* file=fopen(path,"rb");
        fread(&commitdata,sizeof(commitdata),1,file);
        fclose(file);
        strcpy(id[i],commitdata.previous_id);
    }
    while (i--){
        char folderpath[_MAX_PATH];
        sprintf(folderpath,"%s\\commit\\%s",neogitpath,id[i]);
        ocopyfolder(folderpath,newpath);
    }
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
        struct Commitdata commitdata;
        char msg[73];
        sprintf(msg,"branch \"%s\" created",argv[0]);
        initcommitdata(&commitdata,msg);
        strcpy(commitdata.branch,argv[0]);
        char Path[MAX_PATH];
        sprintf(Path,"%s\\commits\\%s",neogitpath,commitdata.id);
        commitdata.fileschanged=0;
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
    char id[ID_LEN+1];
    int argcc=0;
    char msg[73]="";
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
    strcpy(id,argv[argcc+1]);
    
    char path[_MAX_PATH];
    sprintf(path,"%s\\lastcommit",neogitpath);
    deleteinsidefolder(path);
    revert_files(path,id);

    char Path[_MAX_PATH];
    strcpy(Path,neogitpath);
    Path[strlen(neogitpath)-strlen("\\.neogit")]='\0';
    deleteinsidefolder(Path);
    normalcopyfolder(path,Path);
    strcat(Path,"\\commitdata");
    remove(Path);
}