/*
Day9 题3：typedef 别名与函数接口

要求：
1. 用 typedef 给结构体起别名：Student。
2. 用 typedef 给枚举起别名：Grade。
3. 写两个函数：
   - void print_student(const Student *s);
   - void update_score(Student *s, float delta);
4. 在 main 中演示：
   - 打印更新前信息
   - 调用 update_score
   - 打印更新后信息

验收：
- 只读参数使用 const。
- 通过指针修改结构体字段成功。
*/
#include<stdio.h>
typedef enum{A,B,C}Grade;
typedef struct {
   int score;
   Grade g;
}Student;
void print_student(const Student *s){
   printf("score %d grade%d\n",s->score,s->g);
};
Grade getGrade(int score){
if(score>80){return A;};
if(score>60){return B;};
return C;
}
void update_score(Student *s, int delta){
   s->score=s->score+delta;
   s->g=getGrade(s->score);
};
int main(void){
   Student s={
      .score=50,
      .g=C
   };
   print_student(&s);
   update_score(&s,35);
   print_student(&s);
   return 0;
}