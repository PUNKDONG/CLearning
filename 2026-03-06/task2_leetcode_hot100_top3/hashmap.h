#ifndef HASHMAP_H
#define HASHMAP_H
#include<stdlib.h>
 
/* TODO: 定义节点结构体 */
typedef struct HMNode{
    int key;
    int val;
   struct  HMNode*next;
} HMNode;

/* TODO: 定义哈希表结构体 */
typedef struct HashMapInt{
    int cap;
    struct HMNode **buckets;
} HashMapInt;

/* 字符串转 int 哈希值：可把排序后的字符串先压成 int key 再放进现有 map */
int hm_hash_str_value(const char *s){
    unsigned int h = 0;
    while (*s != '\0') {
        h = h * 131u + (unsigned char)(*s);
        s++;
    }
    return (int)h;
}

/* TODO: 计算桶下标 */
int hm_hash_int(int key, int cap){
  return (int)((unsigned int)key % (unsigned int)cap);
}

HashMapInt *hm_create_int(int cap){
  
   HashMapInt *map=( HashMapInt*)malloc(sizeof(HashMapInt));
   HMNode**head=(HMNode **)calloc((size_t)cap, sizeof(HMNode *));
   map->buckets =head;
   map->cap = cap;
   return map;
}

/* TODO: 插入或更新 key -> val */
int hm_put_int(HashMapInt *m, int key, int val){
        if (m == NULL || m->buckets == NULL || m->cap <= 0) {
        return -1;
    };

   int idx = hm_hash_int(key, m->cap);
   HMNode* cur= m->buckets[idx];
   while(cur!=NULL){
    if(cur->key==key){
        cur->val=val;
        return 0;
    }
      cur=cur->next;
   }
   HMNode* newnode=(HMNode*)malloc(sizeof(HMNode));
   newnode->key=key;
   newnode->val=val;
   newnode->next=m->buckets[idx];
   m->buckets[idx]=newnode;

    return 0;   
}

/* TODO: 查询 key，找到写入 *out 并返回 1，没找到返回 0 */
int hm_get_int(HashMapInt *m, int key, int *out){
    int idx=hm_hash_int(key,m->cap);
    HMNode* cur=m->buckets[idx];
    while(cur!=NULL){
       if(cur->key==key){
        *out=cur->val;
        return 1;
       }
       cur=cur->next;
    }

    return 0;

}

/* TODO: 释放整张表 */
void hm_free_int(HashMapInt *m){
    if(m==NULL){
        return;
    }
    for(int i=0;i<m->cap;i++){
        HMNode*cur=m->buckets[i];
        while(cur!=NULL){
            HMNode *next=cur->next;
            free(cur);
            cur=next;
        }
    }
    free(m->buckets);
    free(m);
}

#endif
