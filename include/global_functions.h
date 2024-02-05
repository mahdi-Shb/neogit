#ifndef _GLOBAL_FUNCIONS_H_
#define _GLOBAL_FUNCTIONS_H_
#include <dirent.h>
#include <constant.h>
struct Commitdata{
    // char committype;//c:commit //s:stage //u:unstage
    char id[ID_LEN+1],branch[100];
    char msg[73],username[50],previous_id[ID_LEN+1];
    int fileschanged;
};
struct Shmsg{
    char sh[27],msg[73];
};
struct Alicmd{
    char ali[100],cmd[250];
};
extern char *neogitpath;
extern FILE *stagefile; 
extern char defaultgmail[100],defaultname[100];
extern int numberofcommits;
// extern int numberofstages;
void FindNeoGitDir();
char **readlines(FILE* file);
boolean fileexist(const char*path);
boolean IsStage(const char *path);
// boolean commitexists(char *path);
boolean match_wildcard(const char* WCName,const char* name);
boolean fsame(FILE* f1, FILE* f2);
boolean psame(char* path1, char* path2);
void getneogitpath(char *newpath,const char *path,const char *neogitsubpath);
void getnotneogitpath(char *newpath,const char *path,const char *neogitsubpath);
void deletefolder(char* path);
char *get_file(char *path);
int GlobalInit();
void get_ID(char *);
char getY(char* path,char*subneogitpath);
void fnprintf(FILE* file, const char *str, int d);
// boolean oopen(FILE*file);
FILE* findshortcut(const char* shortcut);
FILE* findalias(const char* alias);
boolean find_commit_file(char *findpath,const char* subneogitpath,const char* path, int i);
int copy_commit_file(char *newpath,char *path,char *subneogitcommitpath,int I);
int ocopyfile(char*path,char*newpath);
int ocopyfolder(char*path,char*newpath);
boolean ncopyfile(char*path,char*newpath);
boolean ncopyfolder(char*path,char*newpath);
#endif