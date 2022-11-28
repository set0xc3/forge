FR_API inline void 
lld_remove(void *list_ptr, void *node_ptr)
{
	Node *node = (Node *)node_ptr;
	LinkedList *list = (LinkedList *)list_ptr;
	
	if (list->head == node)
		list->head = node->next;
	
	if (list->tail == node)
		list->tail = node->prev;
	
	if (node->next)
		node->next->prev = node->prev;
	
	if (node->prev)
		node->prev->next = node->next;
}

FR_API inline void 
lld_push_back(void *list_ptr, void *node_ptr)
{
	Node *node = (Node *)node_ptr;
	LinkedList *list = (LinkedList *)list_ptr;
	
	if (list->head != node ||
		list->tail != node)
		
	{    
		if (!list->head)
		{
			list->head = list->tail = node;
		}
		else
		{
			node->prev = list->tail;
			list->tail->next = node;
			list->tail = node;
		}
	}
}

FR_API inline void 
lld_push_front(void *list_ptr, void *node_ptr)
{
	Node *node = (Node *)node_ptr;
	LinkedList *list = (LinkedList *)list_ptr;
	
	if (list->head != node ||
		list->tail != node)
	{	
		if (!list->head)
		{
			list->head = list->tail = node;
		}
		else
		{
			node->next = list->head;
			list->head->prev = node;
			list->head = node;
		}
	}
}