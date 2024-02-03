#ifndef _GLOBAL_FUNCIONS_H_
#define _GLOBAL_FUNCTIONS_H_
#include <dirent.h>
extern char *neogitpath;
extern FILE *stagefile; 
extern char defaultgmail[100],defaultname[100];
extern int numberofcommits;
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
char getY(char* path);
#endif