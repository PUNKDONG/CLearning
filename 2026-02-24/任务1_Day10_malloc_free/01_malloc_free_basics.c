/*
Day10 练习1：malloc/free 基础

要求：
1. 从标准输入读取一个整数 n (n > 0)。
2. 使用 malloc 在堆上分配一个 int 数组，长度为 n。
3. 用 for 循环给数组赋值：a[i] = i * 2。
4. 计算数组元素总和并打印。
5. free 释放内存，指针置为 NULL。

边界条件：
- 如果 malloc 返回 NULL，打印错误并直接退出。
- 读入失败时，打印错误并退出。

提示：
- malloc 的单位是字节：n * sizeof(int)。

*/
#include<stdio.h>
#include <stdlib.h>
int main(void){
    int n=0; 
    if (scanf("%d", &n) != 1) {
    printf("input error\n");
    return 1;
    }
    if(n<=0){
        return 1;
    }
    int*arr=malloc(sizeof(int)*n);
    if(!arr) {
        printf("failed");
        return 1; 
    } 
    for(int i=0;i<n;i++){
        arr[i]=i*2;
    }
    int sum=0;
    for(int i=0;i<n;i++){
        sum=sum+*(arr+i);
    }
    for(int i=0;i<n;i++){
        printf("%d ",*(arr+i));
    }
    printf("\n");
    printf("sum is %d\n",sum);
    
    free(arr);
    arr=NULL;
    return 0;
}
