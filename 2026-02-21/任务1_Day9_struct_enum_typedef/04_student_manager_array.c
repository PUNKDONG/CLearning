/*
Day9 题4：结构体数组管理（小管理器）

要求：
1. 使用结构体数组保存至少 5 名学生。
2. 写函数实现：
   - 按 id 查找学生（返回下标，找不到返回 -1）
   - 按姓名前缀查找并打印（例如输入 "zh"）
   - 统计及格率（>=60）
3. 在 main 中构造测试数据并验证。

验收：
- 查找与统计结果正确。
- 对“查无此人”有明确输出。
*/
#include<stdio.h>
#include <string.h>

typedef struct{
   int id;
   char* name;
   int score;
}Student;
int findStudent(int id,Student s[],size_t len){
   for(size_t i=0;i<len;i++){
      if(s[i].id==id){
         return i;
      }
   }
   printf("查询失败\n");
   return -1;
} 
size_t findStudentsByname(char* name,Student s[],size_t len,Student out[]){
   size_t count=0;
   for(size_t i=0;i<len;i++){
      if(strncmp(s[i].name,name,strlen(name)) == 0){
         out[count]=s[i];
         count++;
      }
   }
   if(count==0){
   printf("查询失败\n");
   }
   return  count;
} 
float countjige(Student s[],size_t len){
   int count=0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
   for(size_t i=0;i<len;i++){
      if(s[i].score>=60){
         count++;
      }
   }
   return  (float)count/(float)len;
} 
int main(void){ 
     Student students[] = {
        {.id = 1, .name = "zhangsan", .score = 88},
        {.id = 2, .name = "zhouxin", .score = 76},
        {.id = 3, .name = "lisi", .score = 92},
        {.id = 4, .name = "wangwu", .score = 59},
        {.id = 5, .name = "zhaoqi", .score = 81},
    };
   size_t n = sizeof(students) / sizeof(students[0]);
   Student out[10];

   printf("=== 按 id 查找测试 ===\n");
   int idx = findStudent(3, students, n);
   if(idx >= 0){
      printf("find id=3 -> index=%d, name=%s, score=%d\n",
             idx, students[idx].name, students[idx].score);
   }
   idx = findStudent(100, students, n);
   if(idx == -1){
      printf("id=100 not found\n");
   }

   printf("\n=== 按前缀查找测试：zh ===\n");
   size_t found = findStudentsByname("zh", students, n, out);
   for(size_t i=0;i<found;i++){
      printf("id=%d name=%s score=%d\n", out[i].id, out[i].name, out[i].score);
   }

   printf("\n=== 按前缀查找测试：li ===\n");
   found = findStudentsByname("li", students, n, out);
   for(size_t i=0;i<found;i++){
      printf("id=%d name=%s score=%d\n", out[i].id, out[i].name, out[i].score);
   }

   printf("\n=== 按前缀查找测试：xx ===\n");
   found = findStudentsByname("xx", students, n, out);
   if(found == 0){
      printf("prefix xx not found\n");
   }

   printf("\n=== 及格率测试 ===\n");
   printf("pass rate: %.2f%%\n", countjige(students, n) * 100.0f);

   return 0;
}
