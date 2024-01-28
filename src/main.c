#include <stdio.h>
#include "header.h"
int main(int argc, char const *argv[]){
    for (int i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    return 0;
}