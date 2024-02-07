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
#include "constant.h"
const char *COMMAND_NAMES[NUMBER_OF_COMMANDS]={"config","init","reset","status","commit","set","replace","remove","add","log","branch","revert","tag","grep"};
int (*COMMAND_FUNCTIONS[NUMBER_OF_COMMANDS])(int argc,const char* argv[]) ={Cconfig,Cinit,Creset,Cstatus,Ccommit,Cset,Creplace,Cremove,Cadd,Clog,Cbranch,Crevert,Ctag,Cgrep};
char* neogitpath=NULL;
int numberofcommits;
void get_ID(char *ID){
    // srand(time(0));
    // int a=rand();
    int id=numberofcommits+1;
    sprintf(ID,"%d",id);
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
            closedir(neogitdir);
        }
        sprintf(path+i,"..\\.neogit");
    }
}
int GlobalInit(){
    FindNeoGitDir();
    if (neogitpath){
        char A[MAX_PATH];
        sprintf(A,"%s\\commitdatas",neogitpath);
        FILE* file=fopen(A,"r");
        if (file){
            fseek(file,0,SEEK_END);
            numberofcommits=ftell(file)/sizeof(struct Commitdata);
            fclose(file);
        } else {
            printf("Can't open numberofcommits file");
            return 1;
        }
        fclose(file);
        
    }
    return 0;
}
boolean mwc(const char* name,char* tok,boolean starend){
    if (tok==NULL){
        return *name=='\0' || starend ;
    }
    char* s=strstr(name,tok);
    char *Tok=strtok(NULL,WCSTR);
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
    if (wcname[0]!=WCCHR){
        tok=strtok(wcname,WCSTR);
        if (strncmp(name,tok,strlen(tok))){
            return FALSE;
        } else {
            name+=strlen(tok);
            tok=strtok(NULL,WCSTR);
        }
    } else {
        tok=strtok(wcname,WCSTR);
    }
    return mwc(name,tok,WCName[strlen(WCName)-1]==WCCHR);
}
char *get_file(char *path){
    for (char *result=path+strlen(path)-1; result>=path;result--){
        if (*result=='\\'){
            memmove(result+2,result+1,strlen(result+1)+1);
            *(result+1)='\0';            
            return result+2;
        }
    }
    return path;
}
boolean fileexist(const char*path){
    struct stat fileStat;
    return (stat(path, &fileStat) == 0);
}
boolean IsStage(const char *path){
    char newpath[MAX_PATH];
    getneogitpath(newpath,path,"\\stage");
    return fileexist(newpath);
}

boolean fsame(FILE* f1, FILE* f2){
    int s1,s2;
    fseek(f1,0,SEEK_END);
    s1=ftell(f1);
    fseek(f1,0,SEEK_SET);
    fseek(f2,0,SEEK_END);
    s2=ftell(f2);
    fseek(f2,0,SEEK_SET);
    if (s1!=s2) {
        return false;
    }
    if (s1==0) {
        return true;
    }
    char str1[s1+1],str2[s1+1];
    str1[s1]='\0';
    str2[s1]='\0';
    fread(str1,1,s1+1,f1);
    fread(str2,1,s1+1,f2);
    return !strcmp(str1,str2);
}
boolean psame(char* path1, char* path2){
    FILE* f1 = fopen(path1,"r");
    FILE* f2 = fopen(path2,"r");
    if (!f1) return false;//printf("psamefunction:error opening file %s\n",path1);
    if (!f2) return false;//printf("psamefunction:error opening file %s\n",path2);
    boolean a=fsame(f1,f2);
    fclose(f1);
    fclose(f2);
    return a;
}
void getneogitpath(char* newpath,const char* path,const char* neogitsubpath){
    sprintf(newpath,"%s%s%s",neogitpath,neogitsubpath,path+strlen(neogitpath)-strlen("\\.neogit"));
}
void getnotneogitpath(char* newpath,const char* path,const char* neogitsubpath){//neogitsubpath: \commit\123456789
    strcpy(newpath,path);
    memmove(newpath+strlen(neogitpath)-strlen("\\.neogit"),newpath+strlen(neogitpath)+strlen(neogitsubpath),1+strlen(newpath+strlen(neogitpath)+strlen(neogitsubpath)));
}
// void getnormalpath(char* newpath,const char* path,const char* neogitsubpath){
//     if (!strncmp(path,neogitpath,strlen(neogitpath))){
//         //char p[MAX_PATH];
//         strcpy(newpath,neogitpath);
//         sprintf(newpath+strlen(neogitpath)-strlen("\\.neogit"),"%s",strchr(path+strlen(neogitpath)+1,'\\'));
//         getnotneogitpath(newpath,path,neogitsubpath);
//     } else{
//         strcpy(newpath,path);
//     }
// }
char getY(char *path,char* subneogitpath){
    char Path[MAX_PATH];
    getneogitpath(Path,path,subneogitpath);
    FILE* commitFile=fopen(Path,"rb");
    FILE* file=fopen(path,"rb");
    if(!commitFile){
        fclose(file);
        return 'A';
    }
    if (!file){
        fclose(commitFile);
        return 'D';
    }
    if (!fsame(file,commitFile)){
        fclose(file);
        fclose(commitFile);
        return 'M';
    } else {
        fclose(file);
        fclose(commitFile);
        return '\0';
    }
}
void fnprintf(FILE* file,const char* s,int d){
    fprintf(file,"%s",s);
    int j=strlen(s);
    char a='\0';
    while(d-- > j){
        fputc('\0',file);
    }
}
