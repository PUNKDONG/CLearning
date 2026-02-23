/*
Day5 题目3：安全拷贝函数（不越界）

要求：
1. 自己实现函数：
   size_t safe_strcpy(char *dst, size_t dst_size, const char *src);
2. 规则：
   - 将 src 拷贝到 dst
   - 最多拷贝 dst_size - 1 个字符
   - 保证 dst 最后以 '\0' 结尾（当 dst_size > 0 时）
   - 返回 src 的总长度（不是实际拷贝长度）
3. 在 main 中做至少 3 组测试：
   - 目标缓冲区足够大
   - 目标缓冲区偏小（触发截断）
   - dst_size 为 0
4. 每组测试打印：
   - src
   - dst
   - 返回值
   - 是否发生截断（可用 返回值 >= dst_size 判断）

提示：
- 这是嵌入式里非常实用的字符串安全习惯，核心目标是“永不越界”。
*/
#include<stdio.h>
size_t safe_strcpy(char *dst, size_t dst_size, const char *src){
  
   size_t len=0;
   while(src[len]!='\0'){
      len++;
   }
    if(dst_size==0){
      return len;
   }
   size_t copy_len = (len < dst_size - 1) ? len : (dst_size - 1);
   for(size_t i=0;i<copy_len;i++){
      dst[i]=src[i];
   }
   dst[copy_len]='\0';

   return len;
}
int main(void) {
    // 案例1：只定义，不初始化；交给函数写入（正确用法）
    char dst1[20]={0};
    char src[]="123456789012345678";
    size_t returnLen1=safe_strcpy(dst1, sizeof(dst1), src);
    printf("src is %s,dst is %s,src len is %zu,是否截断 %s\n",src,dst1,returnLen1,(returnLen1>=sizeof(dst1))?"YES":"NO");
 
     char dst2[8]={0};
 
    size_t returnLen2=safe_strcpy(dst2, sizeof(dst2), src);
    printf("src is %s,dst is %s,src len is %zu,是否截断 %s\n",src,dst2,returnLen2,(returnLen2>=sizeof(dst2))?"YES":"NO");
 
   char dst3[1]={0};
   size_t dst3_size = 0;
   size_t returnLen3=safe_strcpy(dst3, dst3_size, src);
   printf("src is %s,dst is %s,src len is %zu,是否截断 %s\n",src,dst3,returnLen3,(returnLen3>=dst3_size)?"YES":"NO");
 
    return 0;
}
