#ifndef F_ARRAY_H
#define F_ARRAY_H

/* 
stride
size
capasity
offset

|   |    |    |    |    |    |
| 0 | 8  | 16 | 24 | 32 | 40 |
 | 8 | 16 | 24 | 32 | 40 | 48 |

| object | object | object |    |    |    |
| 0      | 8      | 16     | 24 | 32 | 40 |
 | 8      | 16     | 24     | 32 | 40 | 48 |


size:     3
capasity: 6
stride:   4
offset:   16 + stride

*/

#define ARRAY_DEFAULT_CAPACITY 1
#define ARRAY_RESIZE_FACTOR 2

FR_API void   *f_array_create(u64 capacity, u64 stride);
FR_API void    f_array_destroy(void *array_ptr);

FR_API u64     f_array_size(void *array_ptr);
FR_API u64     f_array_capacity(void *array_ptr);

FR_API void   *f_array_at(void *array_ptr, u64 index, u64 stride);
FR_API void    f_array_push(void *array_ptr, void *value_ptr, u64 stride);
FR_API void    f_array_pop(void *array_ptr, u64 stride);
FR_API void    f_array_insert(void *array_ptr, u64 index, void *value_ptr); //TODO
FR_API void    f_array_delete(void *array_ptr, u64 index); //TODO
FR_API u64     f_array_find(void *array_ptr, void *value_ptr); //TODO

#define array_create(type) \
f_array_create(ARRAY_DEFAULT_CAPACITY, sizeof(type))

#define array_reserve(capacity, type) \
f_array_create(capacity, sizeof(type))

#define array_destroy(array_ptr) \
f_array_destroy(array_ptr)

#define array_size(array_ptr) \
f_array_size(array_ptr)

#define array_capacity(array_ptr) \
f_array_capacity(array_ptr)

#define array_at(array_ptr, index, type) \
f_array_at(array_ptr, index, sizeof(type))

#define array_push(array_ptr, value_ptr, type) \
f_array_push(array_ptr, value_ptr, sizeof(type))

#define array_pop(array_ptr, type) \
f_array_pop(array_ptr, sizeof(type))



#endif //F_ARRAY_H