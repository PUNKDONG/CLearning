/*
LeetCode Hot100 - 题1：两数之和 (Two Sum)

题目描述：
给定一个整数数组 nums 和一个整数目标值 target，
请你在该数组中找出 和为目标值 target 的那两个整数，
并返回它们的数组下标。

你可以假设每种输入只会对应一个答案。
你不能重复利用这个数组中同样的元素。

函数签名（LeetCode C 版）：
int* twoSum(int* nums, int numsSize, int target, int* returnSize);

参数说明：
- nums: 输入数组
- numsSize: 数组长度
- target: 目标和
- returnSize: 输出参数，设置为返回数组长度（应为 2）

返回要求：
- 返回一个动态分配的 int 数组，长度为 2
- returnSize 必须被设置为 2

示例：
输入: nums = [2,7,11,15], target = 9
输出: [0,1]

边界提示：
- numsSize >= 2
- 元素可能为负数
- 注意时间复杂度（你可以先写暴力，再思考优化）
*/

#include <stdlib.h>
#include "hashmap.h"
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize=2;
    int* result=malloc(sizeof(int)*2);
    HashMapInt* map=hm_create_int(32);
    for(int i=0;i<numsSize;i++){
        hm_put_int(map,nums[i],i);
    }
     for(int i=0;i<numsSize;i++){
        int now=nums[i];
        int find=target-now;
        int out=0;
        if(hm_get_int(map,find,&out)){
            if(out==i)continue;
            result[0]=i;
            result[1]=out;
            hm_free_int(map);
            return result;
        }
    }
    return result;
}
