#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
void printcommitdata(struct Commitdata* cd);
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
boolean hastheword(const char*s,const char*word){
    char S[strlen(s)+1];
    strcpy(S,s);
    for (char* token = strtok(S," ?!.,()"); token ;token = strtok(NULL," ?!.,()")){
        if (!strcmp(token,word)){
            return true;
        }
    }
    return false;
}
boolean search(int argc,const char **argv,char *msg){
    for (int j=0;j<argc;j++){
        // printf(argv[j]);
        if (!strchr(argv[j],'*')){
            if (!hastheword(msg,argv[j])){
                return false;
            }
        } else {
            if (!match_wildcard(argv[j],msg)){
                return false;
            }
        }
        
    }
    return true;
}
void printcommitdata(struct Commitdata* cd){
    printf("Time: %s",ctime(&cd->T));
    printf("Message: %s\n",&cd->msg);
    printf("Username: %s\n",cd->username);
    printf("ID: %s\n",cd->id);
    printf("Branch: %s\n",cd->branch);
    printf("The files that changed:%d \n\n",cd->fileschanged);
}
int Clog(int argc, const char *argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    char path[_MAX_PATH];
    sprintf(path,"%s\\commitdatas",neogitpath);
    FILE* file=fopen(path,"rb");
    struct Commitdata cd;
    if (argc==0){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            printcommitdata(&cd);
        }
        fclose(file);
        return 0;
    } else if (!strcmp(argv[0],"-search")){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (search(argc-1,argv+1,cd.msg)){
                printcommitdata(&cd);
            }
        }
        fclose(file);
        return 0;
    } else if(argc != 2){
        printf("Invalid argument number\n");
        fclose(file);
        return 1;
    }
    if (!strcmp(argv[0],"-n")){
        int n = atoi(argv[1]);
        for (int i=numberofcommits-1;i>=numberofcommits-n;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            printcommitdata(&cd);
        }
    } else if (!strcmp(argv[0],"-branch")){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (!strcmp(cd.branch,argv[2])){
                printcommitdata(&cd);
            }
        }
    } else if (!strcmp(argv[0],"-author")){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (!strcmp(cd.username,argv[2])){
                printcommitdata(&cd);
            }
        }
    } else if (!strcmp(argv[0],"-since")){
        struct tm date;
        time_t timeValue;
        // Parse the date string and convert it to a struct tm
        int year, month, day;
        if (sscanf(argv[1], "%d-%d-%d", &year, &month, &day)==3){
            date.tm_year=year-1900;
            date.tm_mon=month-1;
            date.tm_mday=day;
            date.tm_hour = 0;
            date.tm_min = 0;
            date.tm_sec = 0;
            // Convert the struct tm to a time_t value
            timeValue = mktime(&date);
        } else {
            printf("Error: Invalid date format.\nThe format is Y-m-d");
            return 1;
        }
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (cd.T>timeValue){
                printcommitdata(&cd);
            }
        }
    } else if (!strcmp(argv[0],"-before")){
        struct tm date;
        time_t timeValue;
        // Parse the date string and convert it to a struct tm
        int year, month, day;
        if (sscanf(argv[1], "%d-%d-%d", &year, &month, &day)==3){
            date.tm_year=year-1900;
            date.tm_mon=month-1;
            date.tm_mday=day;
            date.tm_hour = 0;
            date.tm_min = 0;
            date.tm_sec = 0;
            // Convert the struct tm to a time_t value
            timeValue = mktime(&date);
        } else {
            printf("Error: Invalid date format.\nThe format is Y-m-d");
            return 1;
        }
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (cd.T<timeValue){
                printcommitdata(&cd);
            }
        }
    } else {
        printf("invalid");
    }
    fclose(file);
    return 0;
}