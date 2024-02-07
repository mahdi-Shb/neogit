#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "constant.h"
#include "global_functions.h"
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
struct Tag {
    char name[73];
    char commitid[ID_LEN+1];
    char author[100];
    char gmail[100];
    time_t T;
    char msg[73];
};
void inittag(struct Tag* tag,char *msg,char* id,char*name){
    strcpy(tag->msg,msg);
    strcpy(tag->commitid,id);
    strcpy(tag->name,name);
    char path[_MAX_PATH];
    sprintf(path,"%s\\configs\\user",neogitpath);
    FILE*file =fopen(path,"r");
    fscanf(file,"%s\n%s",tag->author,tag->gmail);
    fclose(file);
    tag->T=time(NULL);
}
void printtag(struct Tag* tag){
    printf("\nneogit tag show %s\n",tag->name);
    printf("tag %s\n",tag->name);
    printf("commit %s\n",tag->commitid);
    printf("Author: %s <%s>\n",tag->author,tag->gmail);
    printf("Date: %s",ctime(&tag->T));
    if (*tag->msg){
        printf("Message: %s\n",tag->msg);
    }
}

int Ctag(int argc,const char* argv[]){
    if (argc==0){
        char tagPath[MAX_PATH];
        sprintf(tagPath,"%s\\tags",neogitpath);
        struct Tag filetag;
        FILE* file=fopen(tagPath,"r+b");
        while (fread(&filetag,sizeof(filetag),1,file)){
            printtag(&filetag);
        }
        fclose(file);
        return 0;
    } else if (!strcmp(argv[0],"show")){
        if (argc!=2){
            printf("invalid number of arguments");
            return 1;
        }
        char tagPath[MAX_PATH];
        sprintf(tagPath,"%s\\tags",neogitpath);
        struct Tag filetag;
        FILE* file=fopen(tagPath,"r+b");
        while (fread(&filetag,sizeof(filetag),1,file)){
            if (!strcmp(filetag.name,argv[1])){
                printtag(&filetag);
                fclose(file);
                return 0;
            }
        }
        printf("The tag doesn't exist");
        return 1;

    } else if (!strcmp(argv[0],"-a")){
        boolean f=false; 
        char msg[73]="";
        char id[ID_LEN+1]="";
        char name[73]="";
        argc--;
        argv++;
        while (argc){
            if (!strcmp(argv[0],"-m") && !(*msg)){
                strcpy(msg,argv[1]);
                argc-=2;
                argv+=2;
            } else if (!strcmp(argv[0],"-f") && !f){
                f=true;
                argc--;
                argv++;
            } else if (!strcmp(argv[0],"-c") && !(*id)){
                char Path[MAX_PATH];
                sprintf(Path,"%s\\commits",neogitpath);
                DIR* dir=opendir(Path);
                struct dirent* entry;
                while (entry=readdir(dir)){
                    if (!strcmp(entry->d_name,argv[1])){
                        strcpy(id,entry->d_name);
                    }
                }
                if (!id){
                    printf("The id %s doesn't exists",argv[1]);
                    return 1;
                }
                argc-=2;
                argv+=2;
            } else if (!(*name)){
                strcpy(name,argv[0]);
                argv++;
                argc--;
            } else{
                printf("Invalid keys or argument numbers");
                return 1;
            }
        }
        if (!(*name)){
            printf("you didn't give a name for the tag");
            return 1;
        }
        if (!*id){
            char lcdPath[MAX_PATH];
            sprintf(lcdPath,"%s\\lastcommit\\commitdata",neogitpath);
            FILE* file=fopen(lcdPath,"rb");
            if(!file){
                printf("Ctag: can't open lastcommit commitdata file");
                return 1;
            }
            struct Commitdata cd;
            fread(&cd,sizeof(cd),1,file);
            fclose(file);
            strcpy(id,cd.id);
        }
        char tagPath[MAX_PATH];
        sprintf(tagPath,"%s\\tags",neogitpath);
        struct Tag tag;
        struct Tag filetag;
        inittag(&tag,msg,id,name);
        FILE* file=fopen(tagPath,"r+b");
        fseek(file,0,SEEK_END);
        int size=ftell(file);
        fseek(file,0,SEEK_SET);
        do{
            if (!fread(&filetag,sizeof(tag),1,file)){
                fseek(file,0,SEEK_END);
                fwrite(&tag,sizeof(tag),1,file);
                fclose(file);
                printf("The tag added successfully");
                return 0;
            }
        } while(strcmp(filetag.name,name)<0);
        fseek(file,- (long)sizeof(tag),SEEK_CUR);
        if (!strcmp(filetag.name,name)){
            if (f){
                fwrite(&tag,sizeof(tag),1,file);
                fclose(file);
                printf("Replaced tag successfully");
                return 0;
            } else {
                fclose(file);
                printf("This tag has already existed");
                return 1;
            }
        }
        int ja=ftell(file);
        struct Tag tags[(size-ftell(file))/sizeof(tag)];
        fread(tags,sizeof(tags),1,file);
        fseek(file,ja,SEEK_SET);
        fwrite(&tag,sizeof(tag),1,file);
        fwrite(tags,sizeof(tags),1,file);
        fclose(file);
        printf("The tag added successfully");
        
    } else{
        printf("invalid command");
        return 1;
    }
    return 0;
}