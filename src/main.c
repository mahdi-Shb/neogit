#include <stdio.h>
#include "header.h"
int main(int argc, char const *argv[]){
    printf("ridam dahanet");
    for (int i = 3; i < argc; i++){
        printf("%s ", argv[i]);
    }
    return 0;
}