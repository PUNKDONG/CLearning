#include<stdio.h>
#include "args.h"
#include "grep.h"
 
int main(int argc, char *argv[])
{ 
    Arg args;
    if(parse_args(argc, argv,&args)){
        return 1;
    }else{
        grep_file(args);
    }
 
    return 0;
}
