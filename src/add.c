#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
void xcopy(char* path,char* filename){
    char cmd[500];
    char fullpath[_MAX_PATH];
    GetFullPathNameA(path,_MAX_PATH,fullpath,NULL);
    if (strncmp(fullpath,neogitpath,strlen(neogitpath)-strlen("\\.neogit"))){
        printf("The path %s is not in the neogit initialized directory",fullpath);
        return;
    }
    sprintf(cmd,"xcopy %s %s\\stage%s\\%s /E /H /C /I",path,neogitpath,fullpath+strlen(neogitpath)-strlen(".neogit"),filename);
    if (system(cmd)){
        printf("Error occurred while adding the file %s",path);
    } else {
        printf("The file %s addded successfully",path);
    }
}
void copy(char* path,char* filename){
    char cmd[500];
    char fullpath[_MAX_PATH];
    GetFullPathNameA(path,_MAX_PATH,fullpath,NULL);
    if (strncmp(fullpath,neogitpath,strlen(neogitpath)-strlen("\\.neogit"))){
        printf("The path %s is not in the neogit initialized directory",fullpath);
        return;
    }
    sprintf(cmd,"copy %s %s\\stage%s\\%s",path,neogitpath,fullpath+strlen(neogitpath)-strlen(".neogit"),filename);
    if (system(cmd)){
        printf("Error occurred while adding the file %s",path);
    } else {
        printf("The file %s addded successfully",path);
    }
}
void addone(const char* PATH){
    char path[strlen(PATH)+1];
    strcpy(path,PATH);
    struct stat st;
    struct dirent* entry;
    char Path[_MAX_PATH];
    GetFullPathNameA(path,_MAX_PATH,Path,NULL);
    if (strncmp(Path,neogitpath,strlen(neogitpath)-strlen("\\.neogit"))){
        printf("The path %s is not in the neogit initialized directory",path);
        return;
    }
    char *wcname=get_file(Path);
    DIR *dir = opendir(Path);
    while ((entry=readdir(dir))!=NULL){
        if (match_wildcard(wcname,entry->d_name) && strcmp(entry->d_name,".neogit")){
            char p[MAX_PATH];
            sprintf(p,"%s\\%s",Path,entry->d_name);
            if (IsStage(p)){
                printf("%s has already added",p);
            } else {
                fprintf(stagefile,"%s\n",p);
                printf("%s added successfully",p);
            }  
            oee;
            // stat(entry->d_name, &st);
            // if (S_ISDIR(st.st_mode)) {
            //     xcopy(path,entry->d_name);
            // } else {
            //     copy(path,entry->d_name);
            // }
        }
    }
}
void writeone(int I,struct dirent* entry,char* path,char* lpath,HANDLE hConsole) {
    sprintf(lpath,"\\%s",entry->d_name);
    struct stat st;
    stat(path,&st);
    boolean Exists=IsStage(path);
    
    if (S_ISDIR(st.st_mode)){
        SetConsoleTextAttribute(hConsole,Exists ? FOLDER_COLOR|FOREGROUND_INTENSITY : FOLDER_COLOR);
    } else{
        SetConsoleTextAttribute(hConsole,Exists ? FILE_COLOR|FOREGROUND_INTENSITY : FILE_COLOR);
    }
    for (int i = 0; i <I*4; i++){
        printf(" ");
    }
    printf("%s",entry->d_name);
    if (S_ISDIR(st.st_mode)){
        printf("\\");
    }
    if (Exists){
        printf(" -staged");
    }
    printf("\n");
}
void Write(int depth, int I, char* path, char* lpath, HANDLE hConsole){
    if (depth==I){
        return;
    }
    DIR *dir = opendir(path);
    struct dirent* entry;
    while ((entry=readdir(dir))){
        if (strcmp(entry->d_name,".neogit") && strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")) {
            writeone(I,entry,path,lpath,hConsole);
            Write(depth,I+1,path,lpath+strlen(lpath),hConsole);
            *lpath='\0';
        }
    }
}
void WWrite(int depth){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char path[MAX_PATH];
    snprintf(path,strlen(neogitpath)-strlen("\\.neogit"),"%s",neogitpath);
    char* lpath=path+strlen(path);
    printf("%s :\n",path);
    Write(depth,0,path,lpath,hConsole);
    SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
}
int Cadd(int argc,const char* argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    fseek(stagefile,0,SEEK_END);
    fprintf(stagefile,"\n");
    if (!strcmp(argv[0],"-f")){
        for (int i=0;i<argc;i++){
            addone(argv[i]);
        }
        return 0;
    } else if (!strcmp(argv[0],"-n")){
        int depth=1;
        char chert;
        if (argc>2){
            printf("Invalid number of arguments\n");
            return 1;
        } else if (argc==2){
            if (sscanf(argv[1],"%d%c",&depth,&chert)!=1){
                printf("Invalid number as depth\n");
                return 1;
            }
        }
        WWrite(depth);
    } else {
        if (argc==0){
            printf("invalid command");
        } else {
            addone(argv[0]);
            if (argc>1){
                printf("if you want to add multiple files use -f option\n");
            }
        }
    }
    return 0;
}
