#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <windows.h>
// #include <dirent.h>
// #include <unistd.h>
// #include <stdbool.h>
#include "header.h"
#include "commands.h"
#include "global_functions.h"
void may_the_c_surprise_us(){
    // FILE* file = fopen("aaa.txt", "wb");
    // fputc('\0',file);
    // fclose(file);
    // file = fopen("aaa.txt", "rb");
    // char a[1];
    // fread(a,1,1,file);
    // if (*a=='\0'){
    //     printf("doroste ke");
    // }
    // if (file != NULL) {
    //     int c;

    //     // Read and print the contents of the file
    //     printf("File contents:\n");
    //     while ((c = fgetc(file)) != EOF) {
    //         putchar(c);
    //     }
    //     printf("\n");

    //     // Check if end-of-file indicator is set
    //     if (feof(file)) {
    //         printf("Reached end of file.\n");
    //     } else {
    //         printf("Read error occurred.\n");
    //     }

    //     // Close the file
    //     fclose(file);
    // } else {
    //     printf("Failed to open file.\n");
    // }

}
int main(int argc, char *argv[]){
    may_the_c_surprise_us();
    // chdir("D:\\testing project");
    // char a[]="add";
    // char b[]=".";
    // argc=3;
    // char const *argv[]={argvv[0],a,b};

    if (GlobalInit()){
        return 1;
    }
    if (argc<2){
        printf("khob alan man chi ro ejra konam barat? :|");
        return 1;
    }
    if (argc!=2){
        printf("chert voroodi dadi dadash");
    }

    FILE* file=findalias(argv[1]);
    if (!file){
        printf("Can't oprn the alias file");
        return 1;
    }
    if (feof(file)){
        printf("chert voroodi dadi dadash");
        return 1;
    }
    struct Alicmd alicmd;
    fread(&alicmd,sizeof(alicmd),1,file);
    printf("the command is: %s\n",alicmd.cmd);
    fclose(file);
    char *argvv[10];
    argc=1;
    for (char* tok=strtok(alicmd.cmd," ");tok;tok=strtok(NULL," ")){
        argvv[argc++]=tok;
    }
    for (int i=0; i<NUMBER_OF_COMMANDS; i++){
        if (!strcmp(argvv[1],COMMAND_NAMES[i])){
            return COMMAND_FUNCTIONS[i](argc-2,argvv+2);
        }
    }
    printf("chert voroodi dadi dadash");
    return 1;
}