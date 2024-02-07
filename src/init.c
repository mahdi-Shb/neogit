#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>

int Cinit(int argc,const char* argv[]){
    if (neogitpath){
        printf("You have already initialized the neogit");
        return 1;
    }
    if (mkdir(".\\.neogit")){
        printf("Failed to create directory for neogit\n");
        return 1;
    }
    // Create the directory
    BOOL result = CreateDirectoryA(".\\.neogit", NULL);
        // Set the hidden attribute for the folder
    DWORD attributes = GetFileAttributesA(".\\.neogit");
    SetFileAttributesA(".\\.neogit", attributes | FILE_ATTRIBUTE_HIDDEN);

    boolean success=TRUE;
    //making files and dirs inside neogit:stage
    FILE* file;
    success=success && !mkdir(".\\.neogit\\configs");
    success=success && !mkdir(".\\.neogit\\lastcommit");
    success=success && !mkdir(".\\.neogit\\commits");
    success=success && !mkdir(".\\.neogit\\stage");
    success=success && !mkdir(".\\.neogit\\laststage");
    
    file=fopen(".\\.neogit\\shortcuts","w");
    success=success && file;
    if (file){
        fclose(file);
    }
    
    file=fopen(".\\.neogit\\tags","w");
    success=success && file;
    if (file){
        fclose(file);
    }
    
    file=fopen(".\\.neogit\\configs\\alias","w");
    success=success && file;
    if (file){
        fclose(file);
    }
    
    file=fopen(".\\.neogit\\commitdatas","w");
    success=success && file;
    if (file){
        fclose(file);
    }
    
    file=fopen(".\\.neogit\\configs\\user","w");
    success=success && file;
    if (!success){
        rmdir(".\\.neogit");
        printf("Failed to initialize neogit");
        return 1;
    }
    fprintf(file,"%s\n%s",DEFAULT_NAME,DEFAULT_GMAIL);
    fclose(file);
    printf("Initialized successfully\n");
    return 0;
}
