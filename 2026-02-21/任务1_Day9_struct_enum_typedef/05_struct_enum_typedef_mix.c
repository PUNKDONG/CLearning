/*
Day9 题5：综合题（struct + enum + typedef）

场景：
你要做一个“学生任务跟踪”最小模块。

要求：
1. 定义：
   - TaskStatus 枚举（TODO/DOING/DONE）
   - StudentTask 结构体（student_id、task_name、status、progress）
   - 使用 typedef 别名
2. 写函数：
   - void set_status(StudentTask *t, TaskStatus s)
   - void update_progress(StudentTask *t, int delta)
   - void print_task(const StudentTask *t)
3. 在 main 中构造 3 条任务，模拟状态流转并打印每一步。
4. 对非法输入做保护：
   - 空指针
   - progress 越界（限制在 0~100）

验收：
- 输出能清楚看出状态变化和进度变化。
- 无 warning。
*/
#include <stdio.h>
typedef enum
{
   TODO,
   DOING,
   DONE
} TaskStatus;
typedef struct
{
   int student_id;
   char *task_name;
   TaskStatus status;
   int progress;
} StudentTask;
void set_status(StudentTask *t, TaskStatus s)
{
   if(t==NULL){
      return;
   }
   t->status = s;
}
void update_progress(StudentTask *t, int delta)
{ if(t==NULL){
      return;
   }
   int num = t->progress;
   if (num + delta > 100)
   {
      t->progress = 100;
      return;
   }
   if (num + delta < 0)
   {
      t->progress = 0;
      return;
   }
   t->progress = num + delta;
   return;
}
void print_task(const StudentTask *t)
{ if(t==NULL){
      return;
   }
   printf("id %d,name %s,status %d,progress %d\n",t->student_id,t->task_name,t->status,t->progress);
}
int main(void) {
    StudentTask t1 = { .student_id = 1, .task_name = "UART", .status = TODO, .progress = 0 };
    StudentTask t2 = { .student_id = 2, .task_name = "I2C",  .status = TODO, .progress = 0 };
    StudentTask t3 = { .student_id = 3, .task_name = "SPI",  .status = TODO, .progress = 0 };

    // 第一步：初始状态
    print_task(&t1);
    print_task(&t2);
    print_task(&t3);

    // 第二步：开始做
    set_status(&t1, DOING); update_progress(&t1, 30); print_task(&t1);
    set_status(&t2, DOING); update_progress(&t2, 50); print_task(&t2);
    set_status(&t3, DOING); update_progress(&t3, 20); print_task(&t3);

    // 第三步：完成
    update_progress(&t1, 70); set_status(&t1, DONE); print_task(&t1);
    update_progress(&t2, 50); set_status(&t2, DONE); print_task(&t2);
    update_progress(&t3, 80); set_status(&t3, DONE); print_task(&t3);

    return 0;
}
