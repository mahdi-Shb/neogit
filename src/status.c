#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
void swriteone(int I,struct dirent* entry,char* path,char* lpath,HANDLE hConsole) {
    sprintf(lpath,"\\%s",entry->d_name);
    struct stat st;
    stat(path,&st);
    if (!S_ISDIR(st.st_mode)){
        char Y=getY(path,"\\lastcommit");
        if (Y){
            for (int i = 0; i <I*4; i++){
                printf(" ");
            }
            printf("%s",entry->d_name);
            printf(" %c",IsStage(path) ? '+' : '-');
            printf("%c\n",Y);
        }
    } else {
        for (int i = 0; i <I*4; i++){
            printf(" ");
        }
        printf("%s\\\n",entry->d_name);
    }
    // boolean Exists=IsStage(path);
    
    // if (S_ISDIR(st.st_mode)){
    //     SetConsoleTextAttribute(hConsole,Exists ? FOLDER_COLOR|FOREGROUND_INTENSITY : FOLDER_COLOR);
    // } else{
    //     SetConsoleTextAttribute(hConsole,Exists ? FILE_COLOR|FOREGROUND_INTENSITY : FILE_COLOR);
    // }
    // printf("%s",entry->d_name);
    // if (S_ISDIR(st.st_mode)){
    //     printf("\\");
    // }
    // if (Exists){
    //     printf(" ");
    // }
    // printf("\n");
}
void sWrite(int I, char* path, char* lpath, HANDLE hConsole){
    // if (depth==I){
    //     return;
    // }
    DIR *dir = opendir(path);
    struct dirent* entry;
    readdir(dir);
    readdir(dir);
    while ((entry=readdir(dir))){
        if (strcmp(entry->d_name,".neogit")){// && strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")) {
            swriteone(I,entry,path,lpath,hConsole);
            sWrite(I+1,path,lpath+strlen(lpath),hConsole);
            *lpath='\0';
        }
    }
    closedir(dir);
}
void sWWrite(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char path[MAX_PATH];
    snprintf(path,strlen(neogitpath)-strlen("\\.neogit"),"%s",neogitpath);
    path[strlen(neogitpath)-strlen("\\.neogit")]='\0';
    char* lpath=path+strlen(path);
    sWrite(0,path,lpath,hConsole);
    //SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
}

int Cstatus(int argc,const char* argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    if (argc != 0) {
        printf("status command doesn't need arguments");
        return 1;
    }
    sWWrite();
    return 0;
    // struct stat st;
    // char path[_MAX_PATH];
    // getcwd(path,_MAX_PATH);
    // char* lpath=path+strlen(path);
    // DIR* dir = opendir(path);
    // struct dirent* entry;
    // readdir(dir);
    // readdir(dir);
    
    // while ((entry = readdir(dir)) != NULL){
    //     sprintf(lpath,"\\%s",entry->d_name);
    //     stat(path,&st);
    //     if (!S_ISDIR(st.st_mode)){
    //         char Y=getY(path,"\\lastcommit");
    //         if (Y){
    //             printf("%s",entry->d_name);
    //             printf(" %c",IsStage(path) ? '+' : '-');
    //             printf("%c",Y);
    //         }
    //     }
    //     *lpath='\0';
    // }
    // closedir(dir);
    // return 0;
}
