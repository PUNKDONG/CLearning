/*
Day14 练习2（进阶）：学生信息写入与读取还原

题目目标：
- 定义 Student 结构体。
- 将结构体数组用 fprintf 写入 .txt 文件。
- 再从文件读取并还原为结构体数据，打印校验。

建议结构体：
- int id;
- char name[32];
- float score;

文件格式建议（每行一条）：
id,name,score
例如：1001,zhangsan,89.50

要求：
1. 写文件：fopen("students.txt", "w") + fprintf。
2. 读文件：fopen("students.txt", "r") + fgets + 解析（sscanf 或 strtok）。
3. 写/读过程都要检查返回值。
4. 任何 fopen/fprintf/fgets/fclose 失败都用 perror 报错。
5. 最终打印“还原后的学生数据”，确认写入和读取一致。

边界条件：
1. 空文件。
2. 某一行格式错误（应提示并跳过或失败退出，策略自定但要一致）。
3. 名字长度接近上限时不越界。

验收标准：
1. 至少写入 3 条学生记录并成功读回。
2. 输出结果与写入数据一致。
3. 编译无 warning。
*/
#include <stdio.h>
#include <errno.h>
typedef struct Student
{
    int id;
    char name[32];
    float score;
}Student;
void myprintf(struct Student *s,FILE *file){
    
    fprintf(file,"id %d name%s %.2f\n",s->id,s->name,s->score);

}
int main(void){
    FILE * file=fopen("student.txt","w+");
    if(file==NULL){
        perror("file open");
        return -1;
    }
    struct Student s1={1,"zhangyuan1",88};
    struct Student s2={2,"zhangyuan2",77.6};
    struct Student s3={3,"zhangyuan3",99.9};
    myprintf(&s1,file);
    myprintf(&s2,file);
    myprintf(&s3,file);
    //读取
    //错误处理ferorr
    ferror(file);
    rewind(file);
    char buf[256];
     while (fgets(buf, sizeof(buf), file) != NULL)
    {//错误处理ferorr
        if (ferror(file))
        {
            perror("fgets");
            return -1;
        }
         //按照student的格式解析
        struct Student s;
        int ret = sscanf(buf, "id %d name%s %f\n", &s.id, s.name, &s.score);
        if (ret != 3)
        {
            printf("format error\n");
            continue;
        }
        printf("id %d name%s %.2f\n", s.id, s.name, s.score);
    }
    return 0;
}