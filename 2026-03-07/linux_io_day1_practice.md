Linux 应用编程训练题

## 今日模块

第 1 章 Linux 下的 C 语言开发
第 2 章 文件 I/O

## 说明

本套题控制在 1 小时左右完成。
建议顺序：先做概念题，再做代码题，最后做编程题。
请直接把答案写在每道题下面的 答案 区域里，写完后再发给我批改。

---

## 一、概念题

### 1

请写出 C 程序从源文件到可执行文件的四个主要阶段，并说明每个阶段的输入与输出分别是什么。

#### 答案
预处理 .c 输出是 .i 预处理后的文件
编译   .i文件 输出 汇编文件.s
汇编 .s 输出是目标文件 .o
链接,将目标文件链接, 然后输出可执行文件

---

### 2

请解释下面三个命令的作用与区别。

- `gcc -E`这个是预处理只输出预处理后的文件
- `gcc -S`这个是变成汇编语言 输出.s
- `gcc -c`这个编译并汇编 输出.o

#### 答案

---

### 3

什么是文件描述符。
为什么 Linux 中普通文件、终端、管道、套接字都可以用读写接口处理。

#### 答案

1.文件描述符就是这个进程打开文件表的一个索引fd 
2.因为这些东西都是调用系统调用,底层调用的东西都是一样的,通过系统调用来操作底层资源
linux中把很多IO资源都抽象成统一的文件接口

---

### 4

请说明下面几个函数成功与失败时返回值通常应该如何判断。

- `open`
- `read`
- `write`
- `close`

#### 答案
open 成功的时候输出fd是一个int ,失败的时候返回-1,并且设置全局变量errno
read和write成功的时候是输出读取或者写入的字节数,然后就是失败就是-1
close成功关闭就是0,失败就是-1 
read 返回0不是失败,而是相当于读到文件末尾
---

## 二、代码分析题

### 5

请写出程序执行后 `a.txt` 的最终内容，并解释原因。

```c
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("a.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, "hello", 5);
    lseek(fd, 0, SEEK_SET);
    write(fd, "X", 1);
    close(fd);
    return 0;
}
```
#### 答案
最后的a.txt是只有Xello
---

### 6

请分析下面程序可能输出什么，并解释原因。

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("A");
    write(1, "B", 1);
    printf("C\n");
    return 0;
}
```

#### 答案
输出BAC
---

### 7

请指出下面代码中至少 4 个问题。

```c
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("data.txt", O_RDONLY);
    char buf[1024];
    read(fd, buf, sizeof(buf));
    write(1, buf, sizeof(buf));
    return 0;
}
```

#### 答案
 没有检查 open 返回值
没有检查 read 返回值
  write应该读read的返回值
  没有close

...

---

## 三、编程题

### 8

写一个 mini cat 程序。

要求如下：

- 从命令行接收一个文件路径
- 使用`open`、`read`、`write`、`close`
- 将文件内容输出到标准输出
- 处理打开失败
- 处理部分写问题

#### 代码
 
```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main(int argc, char *argv[])
{   char *filename=argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        return 1;
    char buf[4096];
    int nread=0;
    nread=read(fd, buf, sizeof(buf));
    if(nread==-1)return 1;

    int n_write=write(1,buf,nread);
    if(n_write==-1)return 1;
 
    close(fd);
    return 0;
}
 
 
```

#### 思路说明
先打开再关闭就行了,权限设置正确就行
---

### 9

写一个 mini cp 程序。

要求如下：

- 从命令行接收源文件和目标文件路径
- 目标文件不存在则创建
- 目标文件已存在则清空重写
- 循环读取，循环写入
- 任一步失败都要正确释放资源
- 复制完成后输出复制的总字节数

#### 代码

```c

#include<unistd.h>
#include<fcntl.h>

int main(int argc,char*argv[]){
    char* input=argv[1];
    char* target=argv[2];
    int fdin=open(input,O_RDONLY);
     if(fdin==-1){
        close(fdin);
       
        return 1; }
    int fdout=open(target,O_RDWR|O_CREAT|O_TRUNC,0644);
        if(fdout==-1){
        close(fdin);
        close(fdout);
        return 1; }
    char buf[4096];
    int n_read=0;
    n_read=read(fdin,buf,sizeof(buf));
    if(n_read==-1){
        close(fdin);
        close(fdout);
        return 1; }
    int n_write=write(fdout,buf,n_read);
    if(n_write==-1)return 1;
    close(fdin);
    close(fdout);
    return 0;;

}
```

#### 思路说明

---

## 四、加压题

### 10

请回答下面两个问题。

1. 如果`read` 返回 0，通常表示什么。
2. 如果一个程序打开了很多文件但没有关闭，最先可能出现什么现象。

#### 答案
读到文件末尾了
一个进程打开文件数有限制,所以文件打开太多不关闭，常见结果是后续 open 失败

---

## 五、自我复盘

请做完后自己先简单复盘。

### 我最不熟的知识点

### 我最容易写错的代码细节
错误处理方面我做的没有那么完善,因为好像处处都会出错,太难了,似乎所有的东西都要先校验再用,实在是太麻烦了

### 我下次想重点加练的方向
