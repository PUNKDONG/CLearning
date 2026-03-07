#include <stdio.h>
#include <stdlib.h>
#include "map_str_strList.h"

char*** groupAnagrams(char** strs, int strsSize,
                      int* returnSize, int** returnColumnSizes) {
    HMmap *map;
    char ***ans;
    Node *cur;
    int i, j = 0;

    *returnSize = 0;
    *returnColumnSizes = NULL;
    if (strsSize == 0) return NULL;

    map = hm_create(strsSize * 2 + 1);
    if (map == NULL) return NULL;

    for (i = 0; i < strsSize; i++) {
        if (hm_put(map, strs[i]) != 0) return NULL;
    }

    ans = (char ***)malloc(sizeof(char **) * strsSize);
    *returnColumnSizes = (int *)malloc(sizeof(int) * strsSize);
    if (ans == NULL || *returnColumnSizes == NULL) return NULL;

    for (i = 0; i < map->cap; i++) {
        for (cur = map->buckets[i]; cur != NULL; cur = cur->next) {
            ans[j] = cur->list.items;
            (*returnColumnSizes)[j++] = cur->list.size;
        }
    }

    *returnSize = j;
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
