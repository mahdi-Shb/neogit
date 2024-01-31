#include <stdio.h>
// #include "header.h"
#include <dirent.h>
#include <unistd.h>
int main(int argc, char const *argv[]){
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