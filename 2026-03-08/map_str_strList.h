#ifndef MAP_STR_STRLIST_H
#define MAP_STR_STRLIST_H
#include <stdlib.h>
#include <string.h>

#define STRLIST_CAP 100
/*
目标：
做一个最小版 hashmap
- key: 字符串
- value: 字符串数组

为什么 value 不能只写成 char**：
- 因为后面要往数组里 add 字符串
- 所以必须同时保存：
  - size: 当前已有多少个字符串
  - cap: 当前容量
*/

typedef struct StrList
{
     char *items[STRLIST_CAP];
    int size;
} StrList;

typedef struct Node
{
    char *key;
    StrList list;
    struct Node *next;
} Node;

typedef struct HMmap
{
    int cap;
    Node **buckets;
} HMmap;

int hm_index(int cap, char *key);
int strlist_add(StrList *list, char *value);

/*
建议接口：
- create: 创建整张表
- get:    按 key 找到对应的字符串组
- put:    如果 key 不存在就新建组
- free:   释放整张表

可选：
- add_to_group: 往某个 key 对应的组里追加字符串
*/
static int cmp_char(const void *a, const void *b) {
    return *(const char *)a - *(const char *)b;
}

static char *make_key(char *s) {
    size_t len = strlen(s);
    char *key = (char *)malloc(len + 1);
    if (key == NULL) {
        return NULL;
    }

    strcpy(key, s);
    qsort(key, len, sizeof(char), cmp_char);
    return key;
}

HMmap *hm_create(int cap)
{
    HMmap *map = (HMmap *)malloc(sizeof(HMmap));
    if (cap <= 0)
    {
        return NULL;
    }
    if (map == NULL)
    {
        return NULL;
    }
    map->cap = cap;
    map->buckets = (Node **)calloc((size_t)cap, sizeof(Node *));
    if (map->buckets == NULL)
    {
        free(map);
        return NULL;
    }
    return map;
}
StrList *hm_get(HMmap *map, char *key)
{
    if (map == NULL || map->buckets == NULL || key == NULL)
    {
        return NULL;
    }
    int index = hm_index(map->cap, key);
    Node *cur = map->buckets[index];
    char *key2 = make_key(key);
    if (key2 == NULL)
    {
        return NULL;
    }
    while (cur != NULL)
    {
        char *key1 = make_key(cur->key);
        if (key1 != NULL && strcmp(key1, key2) == 0)
        {
            free(key1);
            free(key2);
            return &cur->list;
        }
        free(key1);
        cur = cur->next;
    }
    free(key2);
    return NULL;
}
int hm_index(int cap, char *okey)
{   
    char *key = make_key(okey);
    unsigned int h = 0;
    int index;
    if (key == NULL)
    {
        return 0;
    }
    while (*key != '\0')
    {
        h = h * 131u + (unsigned char)(*key);
        key++;
    }
    index = (int)(h % (unsigned int)cap);
    free(key - strlen(okey)); /* corrected below */
    return index;
}

int hm_put(HMmap *map, char *key)
{
    char *key_copy;
    if (map == NULL || key == NULL)
    {
        return -1;
    }
    int index = hm_index(map->cap, key);
    // 从map映射到hash一样的桶
    Node *bucket = map->buckets[index];

    Node *cur = bucket;
    char *key2 = make_key(key);
    if (key2 == NULL)
    {
        return -1;
    }
    while (cur != NULL)
    {
        char *key1 = make_key(cur->key);
        if (key1 != NULL && strcmp(key1, key2) == 0)
        {
            free(key1);
            free(key2);
            strlist_add(&cur->list, key);
            return 0;
        }
        free(key1);
        cur = cur->next;
    }
    free(key2);
    // 如果没有得话,就要新建一个了
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
        return -1;

    key_copy = (char *)malloc(strlen(key) + 1);
    if (key_copy == NULL)
    {
        free(new_node);
        return -1;
    }
    strcpy(key_copy, key);

    new_node->key = key_copy;
    new_node->list.size = 0;
    if (strlist_add(&new_node->list, key) != 0)
    {
        free(new_node->key);
        free(new_node);
        return -1;
    }
    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
    return 0;
}

int strlist_add(StrList *list, char *value)
{
    if (list == NULL || value == NULL)
    {
        return -1;
    }
    if (list->size >= STRLIST_CAP)
    {
        return -1;
    }
    list->items[list->size++] = value;
    return 0;
}

void hm_free(HMmap *map)
{
    int i;
    if (map == NULL)
    {
        return;
    }
    for (i = 0; i < map->cap; i++)
    {
        Node *cur = map->buckets[i];
        while (cur != NULL)
        {
            Node *next = cur->next;
            free(cur->key);
            free(cur);
            cur = next;
        }
    }
    free(map->buckets);
    free(map);
}

#endif
