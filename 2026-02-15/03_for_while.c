/*
题目3：for / while 循环练习

要求：
1. 使用 for 循环，计算 1 到 100 的和，并输出结果。
2. 使用 while 循环，输出 10 到 1（倒序），每个数字一行。
3. 再写一个循环，统计 1 到 100 中偶数的个数，并输出。

进阶（可选）：
1. 不使用公式，使用循环计算 1! + 2! + 3! + ... + 10! 的值并输出。
*/
#include<stdio.h>

int main(void){
    int sum=0;
    for(int i=1;i<=100;i++){
        sum=sum+i;
    }
    printf("%d\n",sum);
    //
    int a=10;
    while(a>0){
        printf("%d \n",a);
        a--;
    }
    //
    int count=0;
    for(int i=1;i<=100;i++){
        if(i%2==0){
            count++;
        }
    }
    printf("%d\n",count);
    
    //
    int last=1;
    int all=0;
    for(int i=1;i<=10;i++){
        last=last*i;
        all=all+last;
    }
    printf("%d\n",all);
    return 0;
}