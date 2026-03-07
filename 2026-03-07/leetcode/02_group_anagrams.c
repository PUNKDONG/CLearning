/*
LeetCode Hot100 - 题2：字母异位词分组 (Group Anagrams)

题目描述：
给你一个字符串数组，请你将字母异位词组合在一起。
可以按任意顺序返回结果列表。

函数签名（LeetCode C 版）：
char*** groupAnagrams(char** strs, int strsSize,
                      int* returnSize, int** returnColumnSizes);

参数说明：
- `strs`：输入字符串数组
- `strsSize`：字符串个数
- `returnSize`：返回分组数量
- `returnColumnSizes`：返回每组的元素个数数组

返回要求：
- 返回一个二维字符串数组 `char***`
- `*returnSize` 必须设置为分组数量
- `*returnColumnSizes` 必须设置为每组元素个数

示例：
输入：`["eat", "tea", "tan", "ate", "nat", "bat"]`
输出：`[["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]`

说明：
- 同一组里的字符串，排序后结果相同
- 返回顺序不要求固定

边界提示：
- `strsSize` 可能为 0
- 可能有空字符串 `""`
- 可能所有字符串都属于不同组
- 可能所有字符串都属于同一组
*/

#include <stdlib.h>

char*** groupAnagrams(char** strs, int strsSize,
                      int* returnSize, int** returnColumnSizes) {
    (void)strs;
    (void)strsSize;
    (void)returnSize;
    (void)returnColumnSizes;

    /* TODO: 你在这里自己实现 */
    return NULL;
}
