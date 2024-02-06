#ifndef _COMMANDS_H_
#define _COMMANDS_H_
#define NUMBER_OF_COMMANDS 10
int Cadd(int argc,const char* argv[]);
int Cconfig(int argc,const char* argv[]);
int Cinit(int argc,const char* argv[]);
int Creset(int argc,const char* argv[]);
int Cstatus(int argc,const char* argv[]);
int Ccommit(int argc,const char* argv[]);
int Cset(int argc,const char* argv[]);
int Creplace(int argc,const char* argv[]);
int Cremove(int argc,const char* argv[]);
int Clog(int argc,const char* argv[]);
extern const char *COMMAND_NAMES[NUMBER_OF_COMMANDS];
extern int (*COMMAND_FUNCTIONS[NUMBER_OF_COMMANDS])(int argc,const char* argv[]);
#endif