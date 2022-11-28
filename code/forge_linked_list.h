#ifndef F_LINKED_LIST_H
#define F_LINKED_LIST_H

FR_API void lld_remove(void *list_ptr, void *node_ptr);
FR_API void lld_push_back(void *list_ptr, void *node_ptr);
FR_API void lld_push_front(void *list_ptr, void *node_ptr);

#endif //F_LINKED_LIST_H