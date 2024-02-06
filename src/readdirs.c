#include <dirent.h>
#include <stdio.h>
#include "constant.h"
#include <stdarg.h>
#include <stdlib.h>
#define READDIRS_MAX_DIRS 4

char *findstr(char* f,char**d_names,int I){
    for (int i=0;i<I;i++){
        if (!strcmp(d_names[i],f)){
            return d_names[i];
        }
    }
    return NULL;
}
char* readdirs(int count, ...) {
    static DIR *dirs[READDIRS_MAX_DIRS] = {NULL};
    // static char *subpath[READDIRS_MAX_DIRS] = {NULL};
    static char *d_names[1000]={NULL};
    static int I=0;
    if (count > 0) {
        for (int i=0; i<I; i++){
            free(d_names[i]);
            d_names[i]=NULL;
        }
        I=0;
        for (int i=0; i<READDIRS_MAX_DIRS; i++){
            closedir(dirs[i]);
            dirs[i]=NULL;
        }
        va_list args;
        va_start(args, count);
        for (int i = 0; i < count; i++) {
            char *p = va_arg(args, char *);
            dirs[i] = opendir(p);
            if (!dirs[i]) {
                printf("readdirs: Error opening directory %s\n", p);
            } else {
                readdir(dirs[i]);
                readdir(dirs[i]);
            }
        }
        va_end(args);
        return NULL;
    }
    struct dirent *entry = NULL;
    // memset(&entry, 0, sizeof(entry));
    for (int i = 0; i < READDIRS_MAX_DIRS; i++) {
        if (dirs[i]) {
            while (entry = readdir(dirs[i])){
                if (!findstr(entry->d_name, d_names,I)) {
                    d_names[I]=malloc(strlen(entry->d_name)+1);
                    strcpy(d_names[I], entry->d_name);
                    return d_names[I++];
                }
            }
            closedir(dirs[i]);
            dirs[i] = NULL;
        }
    }
    return NULL;
}