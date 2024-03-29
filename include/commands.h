#ifndef _COMMANDS_H_
#define _COMMANDS_H_
#define NUMBER_OF_COMMANDS 14
int Ctag(int argc,const char* argv[]);
int Cgrep(int argc,const char* argv[]);
int Crevert(int argc,const char* argv[]);
int Cbranch(int argc,const char* argv[]);
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