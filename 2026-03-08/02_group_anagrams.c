#include <stdio.h>
#include <stdlib.h>
#include "map_str_strList.h"

char*** groupAnagrams(char** strs, int strsSize,
                      int* returnSize, int** returnColumnSizes) {
   //首先将所有的char* str数组存到这个map中
   HMmap*map=hm_create(strsSize);
   for(int i=0;i<strsSize;i++){
    hm_put(map,strs[i]);
   }
   //然后就是遍历map的每个桶,然后每个桶中的所有的*node都遍历,都是一个数组
   char***ans=(char***)malloc(sizeof(char**)*strsSize);
       *returnColumnSizes = (int *)malloc(sizeof(int) * strsSize);
   int j=0;
   for(int i=0;i<map->cap;i++){
     Node*cur=map->buckets[i];
     while(cur!=NULL){
        ans[j]=cur->list.items;
       (*returnColumnSizes)[j] =cur->list.size;
        j++;
        cur=cur->next;
     }
   }
   *returnSize=j;
   return ans;

}

#ifdef LOCAL_TEST
int main(void) {
    char *strs[] = {"eat", "tea", "tan", "ate", "nat", "bat"};
    int strsSize = (int)(sizeof(strs) / sizeof(strs[0]));
    int returnSize = 0;
    int *returnColumnSizes = NULL;
    char ***ans = groupAnagrams(strs, strsSize, &returnSize, &returnColumnSizes);
    int i, k;

    if (ans == NULL) {
        printf("groupAnagrams failed\n");
        return 1;
    }

    for (i = 0; i < returnSize; i++) {
        printf("group %d: ", i);
        for (k = 0; k < returnColumnSizes[i]; k++) {
            printf("%s ", ans[i][k]);
        }
        printf("\n");
    }

    free(returnColumnSizes);
    free(ans);
    return 0;
}
#endif
