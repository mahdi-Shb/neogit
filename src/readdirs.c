#include <dirent.h>
#include <stdio.h>
#include "constant.h"
#include <stdarg.h>
#include <stdlib.h>
#include "global_functions.h"
void opendirs(struct DIRs** dirs, ...) {
    *dirs=(struct DIRs*) calloc(1,sizeof(struct DIRs));
    va_list args;
    va_start(args, dirs);
    char *p;
    for (int i = 0; p=va_arg(args, char *); i++) {
        (*dirs)->dirs[i] = opendir(p);
        if (!((*dirs)->dirs[i])) {
            // printf("readdirs: Error opening directory %s\n", p);
        } else {
            readdir(((*dirs)->dirs)[i]);
            readdir(((*dirs)->dirs)[i]);
        }
    }
    va_end(args);
}
void closedirs(struct DIRs* dirs){
    for (int i=0; i<dirs->I;i++){
        free(dirs->dirs[i]);
    }
    free(dirs);
}
char* readdirs(struct DIRs* dirs) {
    struct dirent *entry = NULL;
    for (int i = 0; i < READDIRS_MAX_DIRS; i++) {
        if (dirs->dirs[i]) {
            while (entry = readdir(dirs->dirs[i])){
                if (Find_Str(dirs->d_names,entry->d_name,dirs->I)==-1) {
                    dirs->d_names[dirs->I]=malloc(strlen(entry->d_name)+1);
                    strcpy(dirs->d_names[dirs->I], entry->d_name);
                    return dirs->d_names[(dirs->I)++];
                }
            }
            closedir(dirs->dirs[i]);
            dirs->dirs[i] = NULL;
        }
    }
    return NULL;
}