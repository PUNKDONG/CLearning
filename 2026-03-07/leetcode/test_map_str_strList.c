#include <stdio.h>
#include "map_str_strList.h"

static void print_list(const char *key, StrList *list) {
    int i;

    if (list == NULL) {
        printf("key=%s not found\n", key);
        return;
    }

    printf("key=%s size=%d:", key, list->size);
    for (i = 0; i < list->size; i++) {
        printf(" %s", list->items[i]);
    }
    printf("\n");
}

int main(void) {
    HMmap *map = hm_create(17);
    StrList *list;

    if (map == NULL) {
        printf("hm_create failed\n");
        return 1;
    }

    if (hm_put(map, "aet") != 0) return 1;
    if (hm_put(map, "eat") != 0) return 1;
    if (hm_put(map, "ant") != 0) return 1;

    list = hm_get(map, "aet");
    print_list("aet", list);

    list = hm_get(map, "ant");
    print_list("ant", list);

    list = hm_get(map, "bat");
    print_list("bat", list);

    hm_free(map);
    return 0;
}
