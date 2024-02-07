#ifndef _GLOBAL_FUNCIONS_H_
#define _GLOBAL_FUNCTIONS_H_
// #define name(...) readdirs(0, __VA_ARGS__, (char *)NULL)
#include <constant.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include "commands.h"
#include <stdlib.h>
#include <time.h>
#include "constant.h"

#define READDIRS_MAX_DIRS 4
struct DIRs {
    DIR *dirs[READDIRS_MAX_DIRS];
    char *d_names[256];
    int I;
};
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
extern char DELETE_TEXT[];
extern char *neogitpath;
extern FILE *stagefile; 
extern char defaultgmail[100],defaultname[100];
extern int numberofcommits;
void FindNeoGitDir();
char **readlines(FILE* file);
int Find_Str(char** Strs,char* str,size_t StrsLen);
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
boolean find_commit_file(char *findpath,const char* path, char * id);
boolean revert_files(char *newpath,char* lastcommitid);
boolean hastheword(const char*s,const char*word);
// int copy_commit_files(char *newpath,char *path,char *subneogitcommitpath,int I);
void crdir(char *path);
int ocopyfile(char*path,char*newpath);
int ocopyfolder(char*path,char*newpath);
int normalcopyfolder(char*path,char*newpath);
boolean ncopyfile(char*path,char*newpath,char*checkpath);
boolean ncopyfolder(char*path,char*newpath,char*checkpath);
boolean isempty(char*path);
void deletefolder(char* path);
void deleteinsidefolder(char* path);
void makedeletefile(char*path);
boolean isdeletefile(char*path);
char* readdirs(struct DIRs* dirs);
void closedirs(struct DIRs* dirs);
void opendirs(struct DIRs** dirs, ...);
int printbranchs();
#endif