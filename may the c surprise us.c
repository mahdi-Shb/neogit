#include <stdio.h>
// #include "header.h"
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>

int main(int argc, char const *argv[]){
    char p[_MAX_PATH];
    GetFullPathNameA("./**",MAX_PATH,p,NULL);
    printf("%s",p);
    struct dirent* entry;
    DIR *dir = opendir(".");
    entry = readdir(dir);
    printf("ridam dahanet");
    char cwd[1024];
    getcwd(cwd,sizeof(cwd)-1);
    printf("%s\n",cwd);
    // char exe_path[1024];
    // readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    // printf("%s\n",exe_path);
    for (int i = 0; i < argc; i++){
        printf("%s ", argv[i]);

    }
    return 0;
}