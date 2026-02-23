#include "math_utils.h"
#include <stdlib.h>
int max2(int a, int b){
if(a>=b){
    return a;
}else{
    return b;
}
}
int min2(int a, int b){
if(a>=b){
    return b;
}else{
    return a;
}
}
int abs_diff(int a, int b){
return abs(a-b);
}