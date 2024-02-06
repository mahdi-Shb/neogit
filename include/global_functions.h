#ifndef _GLOBAL_FUNCIONS_H_
#define _GLOBAL_FUNCTIONS_H_
#define name(...) readdirs(0, __VA_ARGS__, (char *)NULL)
#include <dirent.h>
#include <time.h>
#include <constant.h>
struct Commitdata{
    // char committype;//c:commit //s:stage //u:unstage
    char id[ID_LEN+1],branch[100];
    char msg[73],username[50],previous_id[ID_LEN+1];
    int fileschanged;
    time_t T;
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
void FindNeoGitDir();
char **readlines(FILE* file);
boolean fileexist(const char*path);
boolean IsStage(const char *path);
boolean match_wildcard(const char* WCName,const char* name);
boolean fsame(FILE* f1, FILE* f2);
boolean psame(char* path1, char* path2);
void getneogitpath(char *newpath,const char *path,const char *neogitsubpath);
void getnotneogitpath(char *newpath,const char *path,const char *neogitsubpath);
char *get_file(char *path);
int GlobalInit();
void get_ID(char *);
char getY(char* path,char*subneogitpath);
void fnprintf(FILE* file, const char *str, int d);
FILE* findshortcut(const char* shortcut);
FILE* findalias(const char* alias);
boolean find_commit_file(char *findpath,const char* subneogitpath,const char* path, int i);
void revert_files(char *newpath,char* lastcommitid);
// int copy_commit_files(char *newpath,char *path,char *subneogitcommitpath,int I);
int ocopyfile(char*path,char*newpath);
int ocopyfolder(char*path,char*newpath);
boolean ncopyfile(char*path,char*newpath,char*checkpath);
boolean ncopyfolder(char*path,char*newpath,char*checkpath);
boolean isempty(char*path);
void deletefolder(char* path);
void deleteinsidefolder(char* path);
void makedeletefile(char*path);
boolean isdeletefile(char*path);

char* readdirs(int count,...);
#endif