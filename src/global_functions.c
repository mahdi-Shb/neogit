#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include "global_functions.h"
#include "commands.h"
#include <stdlib.h>
#include <time.h>
const char *COMMAND_NAMES[NUMBER_OF_COMMANDS]={"config","init","reset","undo","status","commit","set","replace","remove","add"};
int (*COMMAND_FUNCTIONS[NUMBER_OF_COMMANDS])(int argc,const char* argv[]) ={Cconfig,Cinit,Creset,Cundo,Cstatus,Ccommit,Cset,Creplace,Cremove,Cadd};
char* neogitpath=NULL;
FILE* stagefile=NULL;
char defaultgmail[100]="...@gmail.com";
char defaultname[100]="...";
char* get_random_ID(){
    srand(time(0));
    int a=rand();
    char *res=(char*)malloc(sizeof(int)+1);
    sprintf(res,"%X",a);
    return res;
}
char **Readlines(FILE* file){
    char **result=calloc(128,sizeof(void*));
    char p[MAX_PATH];
    int count=0;
    while (fgets(p,MAX_PATH,file)){
        result[count]=malloc(strlen(p)+1);
        strcpy(result[count++],p);
        if (!(count&127)){
            realloc(result,count*sizeof(void*));
        }
    }
}
int Find_Str(char** Strs,char* str,size_t StrsLen){
    for (int i=0; i<StrsLen;i++){
        if (!strcmp(Strs[i],str)){
            return i;
        }
    }
    return -1;
}
void FindNeoGitDir(){
    char path[_MAX_PATH] = ".\\.neogit";
    char full_path[_MAX_PATH]="12345678910";
    for (int i=0;strlen(full_path)>10;i+=3){
        GetFullPathNameA(path, _MAX_PATH, full_path, NULL);
        DIR* neogitdir=opendir(full_path);
        if (neogitdir){
            neogitpath=(char*)malloc(sizeof(full_path));
            strcpy(neogitpath,full_path);
            break;
        }
        sprintf(path+i,"..\\.neogit");
    }
}
int GlobalInit(){
    FindNeoGitDir();
    if (neogitpath){
        char A[MAX_PATH];
        sprintf(A,"%s\\stage",neogitpath);
        stagefile=fopen(A,"r+");
        if (stagefile==NULL){
            printf("Can't open stage file");
            return 1;
        }
    }
    return 0;
}
boolean mwc(const char* name,char* tok,boolean starend){
    if (tok==NULL){
        return *name=='\0' || starend ;
    }
    char* s=strstr(name,tok);
    char *Tok=strtok(NULL,"*");
    while (s){
        name=s+strlen(tok);
        if (mwc(name,Tok,starend)){
            return TRUE;
        }
        s=strstr(name,tok);
    }
    return FALSE;
}
boolean match_wildcard(const char* WCName,const char* name){
    char wcname[200];
    strcpy(wcname,WCName);
    char* tok;
    if (wcname[0]!='*'){
        tok=strtok(wcname,"*");
        if (strncmp(name,tok,strlen(tok))){
            return FALSE;
        } else {
            name+=strlen(tok);
            tok=strtok(NULL,"*");
        }
    } else {
        tok=strtok(wcname,"*");
    }
    return mwc(name,tok,wcname[strlen(wcname)-1]=='*');
}
// boolean match_wildcard_path(const char *path_wild,const char *Path){
//     if (!strncmp(path_wild,Path,strlen(Path))){
//         return TRUE;
//     }
//     char new_path_wild[strlen(path_wild)+1];
//     char new_Path[strlen(Path)+1];
//     strcpy(new_Path,Path);
//     strcpy(new_path_wild,path_wild);
//     char *fpw=get_file(path_wild);
//     char *fP=get_file(Path);
//     if (strcmp()
// }
char *get_file(char *path){
    for (char *result=path+strlen(path)-1; result>=path;result--){
        if (*result=='\\'){
            *result='\0';
            return result+1;
        }
    }
    return path;
}
boolean exists(char *path){
    fseek(stagefile, 0, SEEK_SET);
    char Path[MAX_PATH];
    while (fgets(Path,_MAX_PATH,stagefile)){
        if (!strncmp(path,Path,strlen(Path)-1) && strlen(Path)-1){
            return true;
        }
    }
    return false;
}
boolean IsStage(char *path){
    return exists(path);
}