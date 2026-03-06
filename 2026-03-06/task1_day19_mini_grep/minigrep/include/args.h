// #TODO 建议加 include guard（#ifndef/#define/#endif），避免重复包含问题。
#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
typedef struct Arg{
 int i_flag;
 int n_flag;
 const char * word;
 const char * filepath;
} Arg;


int parse_args(int argc, char ** argv,Arg * args);
#endif