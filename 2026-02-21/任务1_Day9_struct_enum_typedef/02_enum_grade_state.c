/*
Day9 题2：enum 表示等级与状态

要求：
1. 定义一个成绩等级枚举（A/B/C/D/F）。
2. 写函数：根据分数返回等级枚举。
3. 再定义一个学习状态枚举（未开始/进行中/已完成）。
4. 在 main 中测试至少 5 个分数，打印：分数 + 等级 + 状态。

提示：
- 等级可用 switch 映射成字符串输出。

验收：
- 边界分数（如 59/60/89/90）结果正确。
*/

#include<stdio.h>
 
enum Grade { GRADE_F, GRADE_D, GRADE_C, GRADE_B, GRADE_A };
enum status { before,processing,down };

enum Grade score_to_grade(int score) {
    if (score >= 90) return GRADE_A;
    if (score >= 80) return GRADE_B;
    if (score >= 70) return GRADE_C;
    if (score >= 60) return GRADE_D;
    return GRADE_F;
}
enum status getstatus(int score) {
    if (score >= 80) return down;
    if (score >= 70) return processing;
    return before;
}
int main(void){ 
    int score=50;
for(int i=0;i<5;i++){
   printf("%d %d %d\n", score,score_to_grade(score), getstatus(score));
    score=score+10;
}
return 0;
}