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
void copyfile(char*path,char*newpath){
    FILE* file = fopen(path,"rb");
    FILE* newfile = fopen(newpath,"wb");
    if (!file || !newfile){
        printf("Copying error");
        return;
    }
    fseek(file,0,SEEK_END);
    char A[ftell(file)];
    fseek(file,0,SEEK_SET);
    fread(A,sizeof(A),1,file);
    fwrite(A,sizeof(A),1,newfile);
    fclose(newfile);
    fclose(file);
}
int ocopyfile(char*path,char*newpath){
    if (psame(path,newpath)){
        return 0;
    }
    remove(newpath);
    printf("h");
    
    if (!isdeletefile(path)){
        printf("|%s|",path);
        copyfile(path,newpath);
    }
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
        if (mkdir(newpath)){
            printf("can't create directory %s",newpath);
            return -1;
        }
    }
    char *lpath=path+strlen(path);
    DIR *dir=opendir(path);
    char *lnewpath=newpath+strlen(newpath);
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
    closedir(dir);
    return fch;
}
//not overwrite copy file
//used for add
void makedeletefile(char*path){
    FILE *f=fopen(path,"w");
    fwrite(DELETE_TEXT,sizeof(DELETE_TEXT),1,f);
    fclose(f);
}
boolean isdeletefile(char*path){
    FILE *f=fopen(path,"r");
    if (!f) return false;
    char dt[sizeof(DELETE_TEXT)];
    fread(dt,sizeof(DELETE_TEXT),1,f);
    boolean idf = feof(f) && !strcmp(dt,DELETE_TEXT);
    fclose(f);
    return idf;
}
boolean ncopyfile(char*path,char*newpath,char*checkpath){
    if (fileexist(path)){
        if (fileexist(newpath)){ // inja gofte boodin age vojood dashte copy nakone
            return false;
        }
        if (!psame(path,checkpath)){
            copyfile(path,newpath);
            return true;
        } else {
            return false;
        }
    } else {
        if (fileexist(checkpath)){
            if (isdeletefile(newpath)){
                return false;
            }
            makedeletefile(newpath);
            return true;
        } else if (fileexist(newpath)){
            remove(newpath);
            return true;
        }
        return false;
    }
}
boolean ncopyfolder(char*path,char*newpath,char *checkpath){
    if (!strcmp(newpath,neogitpath)||!strcmp(path,neogitpath)){
        return false;
    }
    boolean didcopied=false;
    struct stat st;
    stat(path,&st);
    if (!S_ISDIR(st.st_mode)){
        return ncopyfile(path,newpath,checkpath);
    }
    if (!fileexist(newpath)){
        mkdir(newpath);
        didcopied=true;
    }
    char *lpath=path+strlen(path);
    char *lnewpath=newpath+strlen(newpath);
    char *lcheckpath=checkpath+strlen(checkpath);
    DIR *dir=opendir(path);
    // DIR *newdir=opendir(newpath);
    struct dirent *entry;
    readdir(dir);
    readdir(dir);
    while (entry=readdir(dir)){
        sprintf(lpath,"\\%s",entry->d_name);
        sprintf(lnewpath,"\\%s",entry->d_name);
        sprintf(lcheckpath,"\\%s",entry->d_name);
        boolean fl=ncopyfolder(path,newpath,checkpath);
        didcopied=didcopied || fl;
        *lpath='\0';
        *lnewpath='\0';
        *lcheckpath='\0';
    }
    closedir(dir);
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
    closedir(dir);
    rmdir(path);
}
void deleteinsidefolder(char* path){
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
    closedir(dir);
}

boolean isempty(char*path){
    DIR *dir=opendir(path);
    readdir(dir);
    readdir(dir);
    return !readdir(dir);
}