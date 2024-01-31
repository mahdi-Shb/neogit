#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include "global_functions.h"
#include "commands.h"
#include <stdlib.h>
_WDIR* neogitdir;
const char *COMMAND_NAMES[NUMBER_OF_COMMANDS]={"config","init","rest","reset","undo","status","commit","set","replace","remove"};
int (*COMMAND_FUNCTIONS[NUMBER_OF_COMMANDS])(int argc,const char* argv[]) ={Cconfig,Cinit,Crest,Creset,Cundo,Cstatus,Cset,Creplace,Cremove};
void FindNeoGitDir(){
    
    wchar_t path[MAX_PATH] = L".\\.neogit";
    wchar_t full_path[MAX_PATH]=L"12345678910";
    for (int i=0;wcslen(full_path)>10;i+=3){
        GetFullPathNameW(path, MAX_PATH, full_path, NULL);
        neogitdir=wopendir(full_path);
        if (neogitdir){
            break;
        }
    }
}
void GlobalInit(){
    FindNeoGitDir();
}