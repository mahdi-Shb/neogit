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
//overwrite copy file
//used for commits
int ocopyfile(char*path,char*newpath){
    if (psame(path,newpath)){
        return 0;
    }
    remove(newpath);
    char cmd[500];
    sprintf(cmd,"copy %s %s",path,newpath);
    system(cmd);
    return 1;
}
int ocopyfolder(char*path,char*newpath){
    if (!strcmp(newpath,neogitpath)||!strcmp(path,neogitpath)){
        return 0;
    }
    struct stat st;
    stat(path,&st);
    if (!S_ISDIR(st.st_mode)){
        return ocopyfile(path,newpath);
    }
    int fch=0;
    if (!fileexist(newpath)){
        mkdir(newpath);
        fch++;
    }
    char *lpath=path+strlen(path);
    DIR *dir=opendir(path);
    char *lnewpath=newpath+strlen(newpath);
    DIR *newdir=opendir(newpath);
    struct dirent *entry;
    readdir(dir);
    readdir(dir);
    while (entry=readdir(dir)){
        sprintf(lpath,"\\%s",entry->d_name);
        sprintf(lnewpath,"\\%s",entry->d_name);
        fch+=ocopyfolder(path,newpath);
        *lpath='\0';
        *lnewpath='\0';
    }
    return fch;
}
//not overwrite copy file
//used for add
boolean ncopyfile(char*path,char*newpath){
    if (fileexist(newpath)){
        return false;
    }
    char cmd[500];
    sprintf(cmd,"copy %s %s",path,newpath);
    system(cmd);
    return true;
}
boolean ncopyfolder(char*path,char*newpath){
    if (!strcmp(newpath,neogitpath)||!strcmp(path,neogitpath)){
        return false;
    }
    boolean didcopied=false;
    struct stat st;
    stat(path,&st);
    if (!S_ISDIR(st.st_mode)){
        return ncopyfile(path,newpath);
    }
    if (!fileexist(newpath)){
        mkdir(newpath);
        didcopied=true;
    }
    char *lpath=path+strlen(path);
    DIR *dir=opendir(path);
    char *lnewpath=newpath+strlen(newpath);
    DIR *newdir=opendir(newpath);
    struct dirent *entry;
    readdir(dir);
    readdir(dir);
    while (entry=readdir(dir)){
        sprintf(lpath,"\\%s",entry->d_name);
        sprintf(lnewpath,"\\%s",entry->d_name);
        didcopied=didcopied || ncopyfolder(path,newpath);
        *lpath='\0';
        *lnewpath='\0';
    }
    return didcopied;
}
void deletefolder(char* path){
    if (!strcmp(path,neogitpath)){
        return;
    }
    struct stat st;
    stat(path,&st);
    if (!S_ISDIR(st.st_mode)){
        remove(path);
        return;
    }
    char *lpath=path+strlen(path);
    DIR *dir=opendir(path);
    struct dirent *entry;
    readdir(dir);
    readdir(dir);
    while (entry=readdir(dir)){
        sprintf(lpath,"\\%s",entry->d_name);
        deletefolder(path);
        *lpath='\0';
    }
    rmdir(path);
}