/*
Day6 练习4：指针参数与安全检查

要求：
1. 实现函数：
   int increment_if_valid(int *x, int delta);
2. 行为定义：
   - 如果 x == NULL，返回 -1，不做修改
   - 否则对 *x 加上 delta，返回 0
3. 在 main 中做至少 3 组测试：
   - 正常指针（应成功修改）
   - 传入 NULL（应安全返回 -1，不崩溃）
   - 负数 delta（检查逻辑是否正确）
4. 打印每组测试的输入、返回值、修改后结果。

进阶（可选）：
1. 再实现一个函数：
   int set_value(int *x, int value);
   规则同上，练习“通过指针写值 + 空指针保护”。

提示：
- 嵌入式里常见硬件接口函数也会有这种“先判空再操作”的防御式写法。
*/
#include <stdio.h>
int increment_if_valid(int *x, int delta){
   if(x==NULL){
      return -1;
   }
   *x=*x+delta;
   return 0;
} 
int main(void){
   //打印每组测试的输入、返回值、修改后结果。
   int x=10;
   int *xx=NULL;
   int delta1=9;
   int delta2=-10;
   printf("case1 before: x=%d, delta=%d\n",x,delta1);
   int ret1=increment_if_valid(&x,delta1);
   printf("case1 return=%d, after: x=%d\n",ret1,x);

   printf("case2 before: x=%d, delta=%d\n",x,delta2);
   int ret2=increment_if_valid(&x,delta2);
   printf("case2 return=%d, after: x=%d\n",ret2,x);

   int ret3=increment_if_valid(xx,delta1);
   printf("case3 pointer=NULL, delta=%d, return=%d\n",delta1,ret3);
      
   return 0;
}
