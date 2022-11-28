typedef struct FArray
{
	void *data;
	u64 size;
	u64 capacity;
}FArray;

internal void f_array_resize(FArray *array, i64 new_capacity);

internal void 
f_array_resize(FArray *array, i64 new_capacity)
{
	
}

FR_API void 
*f_array_create(u64 capacity, u64 stride)
{
	FArray *result = os_memory_reserve(sizeof(FArray));
	ASSERT(result == 0);
	
	result->size     = 0;
	result->capacity = capacity;
	result->data     = os_memory_reserve(stride * capacity);
	ASSERT(result->data == 0);
	
	return result;
}

FR_API void 
f_array_destroy(void *array_ptr)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	os_memory_release(array->data, array->capacity);
	os_memory_release(array, sizeof(FArray));
}

FR_API u64 f_array_size(void *array_ptr)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	return array->size;
}

FR_API u64 f_array_capacity(void *array_ptr)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	return array->capacity;
}

FR_API void 
*f_array_at(void *array_ptr, u64 index, u64 stride)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	if (index <= array->capacity - 1)
	{
		return (void *)(*(u64 *)((u64)array->data + (index * stride)));
	}
	else
	{
		ASSERT(true);
		return 0;
	}
}

FR_API void 
f_array_push(void *array_ptr, void *value_ptr, u64 stride)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	if (array->size < array->capacity)
	{
		*(u64 *)((u64)array->data + (array->size * stride)) = (u64)value_ptr;
		
		array->size += 1;
	}
	else
	{
		ASSERT(true);
	}
}

FR_API void 
f_array_pop(void *array_ptr, u64 stride)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	if (array->size > 0)
	{
		*(u64 *)((u64)array->data + ((array->size - 1) * stride)) = 0;
		
		array->size -= 1;
	}
	else
	{
		ASSERT(true);
	}
}

FR_API void 
f_array_insert(void *array_ptr, u64 index, void *value_ptr)
{
	ASSERT(array_ptr == 0);
	FArray *array = array_ptr;
	
	if (index <= array->capacity - 1)
	{
		
	}
	else
	{
		ASSERT(true);
	}
}