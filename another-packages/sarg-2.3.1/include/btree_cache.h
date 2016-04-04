#ifndef BTREE_CACHE_H
#define BTREE_CACHE_H

void init_cache(void);
void destroy_cache(void);
char *search_in_cache(const char *key);
int insert_to_cache(const char *key, const char *value);

#endif //BTREE_CACHE_H
