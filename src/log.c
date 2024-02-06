#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

boolean hastheword(const char*s,const char*word){
    char S[strlen(s)+1];
    strcpy(S,s);
    for (char* token = strtok(S," ?!.,()"); token ;token = strtok(NULL," ?!.,()")){
        if (!strcmp(token,word)){
            return true;
        }
    }
    return false;
}
boolean search(int argc,const char **argv,char *msg){
    for (int j=0;j<argc;j++){
        // printf(argv[j]);
        if (!strchr(argv[j],'*')){
            if (!hastheword(msg,argv[j])){
                return false;
            }
        } else {
            if (!match_wildcard(argv[j],msg)){
                return false;
            }
        }
        
    }
    return true;
}
void printcommitdata(struct Commitdata* cd){
    printf("Time: %s",ctime(&cd->T));
    printf("Message: %s\n",&cd->msg);
    printf("Username: %s\n",cd->username);
    printf("ID: %s\n",cd->id);
    printf("Branch: %s\n",cd->branch);
    printf("The files that changed:%d \n\n",cd->fileschanged);
}
int Clog(int argc, const char *argv[]){
    if (!neogitpath){
        printf("You didn't initialize the neogit\n");
        return 1;
    }
    char path[_MAX_PATH];
    sprintf(path,"%s\\commitdatas",neogitpath);
    FILE* file=fopen(path,"rb");
    struct Commitdata cd;
    if (argc==0){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            printcommitdata(&cd);
        }
        fclose(file);
        return 0;
    } else if (!strcmp(argv[0],"-search")){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (search(argc-1,argv+1,cd.msg)){
                printcommitdata(&cd);
            }
        }
        fclose(file);
        return 0;
    } else if(argc != 2){
        printf("Invalid argument number\n");
        fclose(file);
        return 1;
    }
    if (!strcmp(argv[0],"-n")){
        int n = atoi(argv[1]);
        for (int i=numberofcommits-1;i>=numberofcommits-n;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            printcommitdata(&cd);
        }
    } else if (!strcmp(argv[0],"-branch")){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (!strcmp(cd.branch,argv[2])){
                printcommitdata(&cd);
            }
        }
    } else if (!strcmp(argv[0],"-author")){
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (!strcmp(cd.username,argv[2])){
                printcommitdata(&cd);
            }
        }
    } else if (!strcmp(argv[0],"-since")){
        struct tm date;
        time_t timeValue;
        // Parse the date string and convert it to a struct tm
        int year, month, day;
        if (sscanf(argv[1], "%d-%d-%d", &year, &month, &day)==3){
            date.tm_year=year-1900;
            date.tm_mon=month-1;
            date.tm_mday=day;
            date.tm_hour = 0;
            date.tm_min = 0;
            date.tm_sec = 0;
            // Convert the struct tm to a time_t value
            timeValue = mktime(&date);
        } else {
            printf("Error: Invalid date format.\nThe format is Y-m-d");
            return 1;
        }
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (cd.T>timeValue){
                printcommitdata(&cd);
            }
        }
    } else if (!strcmp(argv[0],"-before")){
        struct tm date;
        time_t timeValue;
        // Parse the date string and convert it to a struct tm
        int year, month, day;
        if (sscanf(argv[1], "%d-%d-%d", &year, &month, &day)==3){
            date.tm_year=year-1900;
            date.tm_mon=month-1;
            date.tm_mday=day;
            date.tm_hour = 0;
            date.tm_min = 0;
            date.tm_sec = 0;
            // Convert the struct tm to a time_t value
            timeValue = mktime(&date);
        } else {
            printf("Error: Invalid date format.\nThe format is Y-m-d");
            return 1;
        }
        for (int i=numberofcommits-1;i>=0;i--){
            fseek(file,i*sizeof(cd),SEEK_SET);
            fread(&cd,sizeof(cd),1,file);
            if (cd.T<timeValue){
                printcommitdata(&cd);
            }
        }
    } else {
        printf("invalid");
    }
    fclose(file);
    return 0;
}
int printbranchs(){
    char *branchnames[100];
    struct Commitdata* branchcommits[100];
    int count=0;
    char path[_MAX_PATH];
    sprintf(path,"%s\\commitdatas",neogitpath);
    FILE* file=fopen(path,"rb");
    struct Commitdata cd;
    while(fread(&cd,sizeof(cd),1,file)){
        int i=Find_Str(branchnames,cd.branch,count);
        if (i==-1){
            branchcommits[count]=(struct Commitdata*)malloc(sizeof(struct Commitdata));
            memcpy(branchcommits[count],&cd,sizeof(cd));
            branchnames[count]=(struct Commitdata*)malloc(strlen(cd.branch)+1);
            strcpy(branchnames[count],cd.branch);
            count++;
        } else {
            memcpy(branchcommits[i],&cd,sizeof(cd));
        }
    }
    for (int i=0;i<count;i++){
        printf("branchname: %s\n",branchnames[i]);
        printf("last commitdata:\n");
        printcommitdata(branchcommits[i]);
        free(branchnames[i]);
        free(branchcommits[i]);
    }
    fclose(file);
    return 0;
    
}