#ifndef GREP_H
#define GREP_H
 
#include "args.h"

 
int match_line(const char *buf,const char *word, int i_flag);
void grep_file(Arg args);

#endif
