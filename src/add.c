#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
// void find_deletes(const char* path){
//     static Path;
//     if (path!=NULL){
//         strcpy(Path,path);

//     }

// }
void xcopy(const char* path){
    char cmd[500];
    char newpath[MAX_PATH];
    getneogitpath(newpath,path,"\\stage");
    deletefolder(newpath);
    if (fileexist(newpath)){
        printf("oh shit");
        return;
    }
    sprintf(cmd,"xcopy \"%s\" \"%s\" /E /H /C /I",path,newpath);
    system(cmd);
}
void copy(const char* path){
    char cmd[500];
    char newpath[MAX_PATH];
    getneogitpath(newpath,path,"\\stage");
    if (remove(newpath)){
        printf("Failed to remove %s\n",newpath);
        return;
    }
    if (fileexist(newpath)){
        printf("ooooh shit");
        return;
    }
    sprintf(cmd,"copy \"%s\" \"%s\"",path,newpath);
    system(cmd);
}
void Copy(char* path){
    struct stat st;
    if (stat(path,&st)==0){   
        if (S_ISDIR(st.st_mode)) {
            xcopy(path);
        } else {
            copy(path);
        }
    } else {
        char Path[MAX_PATH];
        getneogitpath(Path,path,"\\stage");
        deletefolder(Path);
    }
    
}
void add_path(char* path){
    char newpath[MAX_PATH];
    getneogitpath(newpath,path,"\\stage");
    char checkpath[MAX_PATH];
    getneogitpath(checkpath,path,"\\lastcommit");
    // printf("checkpath:%s",checkpath);
    // sprintf(Path+strlen(Path),"%d",numberofstages);
    ncopyfolder(path,newpath,checkpath);
}
void reset_path(char* path){
    char Path[MAX_PATH];
    getneogitpath(Path,path,"\\stage");
    // sprintf(Path+strlen(Path),"%d",numberofstages);
    deletefolder(Path);
}
void addone(const char* PATH,boolean a){
    char path[_MAX_PATH];
    GetFullPathNameA(PATH,_MAX_PATH,path,NULL);
    struct stat st;
    struct dirent* entry;
    if (strncmp(path,neogitpath,strlen(neogitpath)-strlen("\\.neogit"))){
        printf("The path %s is not in the neogit initialized directory",path);
        return;
    }

    if (!strncmp(path,neogitpath,strlen(path))){
        strcat(path,".\\*");
        addone(path,a);
        return;
    }
    char Path[_MAX_PATH];
    int n=0;
    DIR *dir;
    char *wcname;

    // getneogitpath(Path,path,"\\stage");
    // wcname=get_file(Path);
    // dir = opendir(Path);
    // readdir(dir);
    // readdir(dir);
    // while ((entry=readdir(dir))!=NULL){
    //     if (match_wildcard(wcname,entry->d_name) && strcmp(entry->d_name,".neogit")){
    //         char p[MAX_PATH];
    //         sprintf(p,"%s%s",Path,entry->d_name);
    //         getnotneogitpath(p,p,"\\stage");
    //         add_path(p);
    //         n++;
    //     }
    // }

    // getneogitpath(Path,path,"\\lastcommit");
    // wcname=get_file(Path);
    // dir = opendir(Path);
    // readdir(dir);
    // readdir(dir);
    // while ((entry=readdir(dir))!=NULL){
    //     if (match_wildcard(wcname,entry->d_name) && strcmp(entry->d_name,".neogit")){
    //         char p[MAX_PATH];
    //         sprintf(p,"%s%s",Path,entry->d_name);
    //         getnotneogitpath(p,p,"\\lastcommit");
    //         add_path(p);
    //         n++;
    //     }
    // }

    strcpy(Path,path);
    wcname=get_file(Path);
    dir = opendir(Path);
    readdir(dir);
    readdir(dir);
    while ((entry=readdir(dir))!=NULL){
        if (match_wildcard(wcname,entry->d_name) && strcmp(entry->d_name,".neogit")){
            char p[MAX_PATH];
            sprintf(p,"%s%s",Path,entry->d_name);
            if (a){
                add_path(p);
            } else {
                reset_path(p);
            }
            n++;
        }
    }
    closedir(dir);
    if (!n){
        printf("There is no matching file or directory with the path %s",PATH);
    }
}
void writeone(int I,struct dirent* entry,char* path,char* lpath,HANDLE hConsole) {
    sprintf(lpath,"\\%s",entry->d_name);
    struct stat st;
    stat(path,&st);
    boolean Exists=IsStage(path);
    
    // if (S_ISDIR(st.st_mode)){
    //     SetConsoleTextAttribute(hConsole,Exists ? FOLDER_COLOR|FOREGROUND_INTENSITY : FOLDER_COLOR);
    // } else{
    //     SetConsoleTextAttribute(hConsole,Exists ? FILE_COLOR|FOREGROUND_INTENSITY : FILE_COLOR);
    // }
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
    closedir(dir);
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
void undo(){
    char path1[MAX_PATH],path2[MAX_PATH],pathtemp[MAX_PATH];
    sprintf(path1,"%s\\stage",neogitpath);
    sprintf(path2,"%s\\laststage",neogitpath);
    sprintf(pathtemp,"%s\\temp",neogitpath);
    ocopyfolder(path1,pathtemp);
    deleteinsidefolder(path1);
    ocopyfolder(path2,path1);
    deleteinsidefolder(path2);
    ocopyfolder(pathtemp,path2);
    deletefolder(pathtemp);
}
int Cadd(int argc,const char* argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    if (argc==0){
        printf("invalid command");
        return 1;
    }
    // fseek(stagefile,0,SEEK_END);
    // fprintf(stagefile,"\n");
    if (!strcmp(argv[0],"-f")){
        char path[MAX_PATH];
        sprintf(path,"%s\\laststage",neogitpath);
        // deletefolder(path);
        // mkdir(path);
        char Path[MAX_PATH];
        sprintf(Path,"%s\\stage",neogitpath);
        ocopyfolder(Path,path);

        for (int i=1;i<argc;i++){
            addone(argv[i],true);
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
    }else if (!strcmp(argv[0],"-redo")){
        if (argc!=1){
            printf("So much arguments!");
            return 1;
        }
        undo();
    } else {
        char path[MAX_PATH];
        sprintf(path,"%s\\laststage",neogitpath);
        // deletefolder(path);
        // mkdir(path);
        char Path[MAX_PATH];
        sprintf(Path,"%s\\stage",neogitpath);
        ocopyfolder(Path,path);

        addone(argv[0],true);
        if (argc>1){
            printf("Only the first path added\nIf you want to add multiple files use -f option");
        }
    }
    return 0;
}
int Creset(int argc,const char* argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    if (argc==0){
        printf("invalid command");
        return 1;
    }
    // fseek(stagefile,0,SEEK_END);
    // fprintf(stagefile,"\n");
    if (!strcmp(argv[0],"-f")){
        char path[MAX_PATH];
        sprintf(path,"%s\\laststage",neogitpath);
        // deletefolder(path);
        // mkdir(path);
        char Path[MAX_PATH];
        sprintf(Path,"%s\\stage",neogitpath);
        ocopyfolder(Path,path);

        for (int i=1;i<argc;i++){
            addone(argv[i],false);
        }
        return 0;
    } else if (!strcmp(argv[0],"-undo")){
        if (argc!=1){
            printf("So much arguments!");
            return 1;
        }
        undo();
    } else {
        char path[MAX_PATH];
        sprintf(path,"%s\\laststage",neogitpath);
        // deletefolder(path);
        // mkdir(path);
        char Path[MAX_PATH];
        sprintf(Path,"%s\\stage",neogitpath);
        ocopyfolder(Path,path);

        addone(argv[0],false);
        if (argc>1){
            printf("Only the first path reset\nIf you want to reset multiple files use -f option");
        }
    }
    return 0;
}