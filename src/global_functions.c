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
#include "constant.h"
const char *COMMAND_NAMES[NUMBER_OF_COMMANDS]={"config","init","reset","undo","status","commit","set","replace","remove","add"};
int (*COMMAND_FUNCTIONS[NUMBER_OF_COMMANDS])(int argc,const char* argv[]) ={Cconfig,Cinit,Creset,Cundo,Cstatus,Ccommit,Cset,Creplace,Cremove,Cadd};
char* neogitpath=NULL;
FILE* stagefile=NULL;
int numberofcommits;
char defaultgmail[100]="...@gmail.com";
char defaultname[100]="...";

void get_ID(char *ID){
    // srand(time(0));
    // int a=rand();
    int id=numberofcommits;
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
        }
        sprintf(path+i,"..\\.neogit");
    }
}
int GlobalInit(){
    FindNeoGitDir();
    if (neogitpath){
        char A[MAX_PATH];
        sprintf(A,"%s\\commits\\specs\\numberofcommits",neogitpath);
        FILE* file=fopen(A,"r");
        if (file){
            fscanf(file,"%d",&numberofcommits);
            fclose(file);
        } else {
            printf("Can't open numberofcommits file");
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
// boolean commitexists(char *path){
//     fseek(stagefile, 0, SEEK_SET);
//     char Path[MAX_PATH];
//     while (fgets(Path,_MAX_PATH,stagefile)){
//         if (!strncmp(path,Path,strlen(Path)-1) && strlen(Path)-1){
//             return true;
//         }
//     }
//     return false;
// }
boolean IsStage(const char *path){
    // return exists(path);
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
    char str1[s1+1],str2[s1+1];
    fread(str1,1,s1+1,f1);
    fread(str2,1,s1+1,f2);
    return !strcmp(str1,str2);
}
boolean psame(char* path1, char* path2){
    FILE* f1 = fopen(path1,"r");
    FILE* f2 = fopen(path2,"r");
    return fsame(f1,f2);
}
void getneogitpath(char* newpath,const char* path,const char* neogitsubpath){
    sprintf(newpath,"%s%s%s",neogitpath,neogitsubpath,path+strlen(neogitpath)-strlen("\\.neogit"));
}
void getnotneogitpath(char* newpath,const char* path,const char* neogitsubpath){//neogitsubpath: \commit\123456789
    strcpy(newpath,path);
    memmove(newpath+strlen(neogitpath)-strlen("\\.neogit"),newpath+strlen(neogitpath)+strlen(neogitsubpath),1+strlen(newpath+strlen(neogitpath)+strlen(neogitsubpath)));
}
char getY(char *path,char* subneogitpath){
    char Path[MAX_PATH];
    //find_commit_file(path)
    getneogitpath(Path,path,subneogitpath);
    FILE* commitFile=fopen(Path,"rb");
    FILE* file=fopen(path,"rb");
    if(!commitFile){
        return 'A';
    }
    if (!file){
        return 'D';
    }
    if (!fsame(file,commitFile)){
        return 'M';
    } else {
        return '\0';
    }
}
void fnprintf(FILE* file,const char* s,int d){
    fprintf(file,"%s",s);
    int j=strlen(s);
    char a='\0';
    while(d-- > j){
        fputc('\0',file);
        // fwrite(&a,1,1,file);
        // fprintf(file,"%c",'\0');
        // fputc('\0',file);
    }
}
