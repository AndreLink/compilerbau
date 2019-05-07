#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct list_element
{
	struct list_element * prev;
	struct list_element * next;
	void * data;
} list_element;


typedef struct linked_list
{
	struct list_element * first;
	struct list_element * last;
} linked_list;


/***
*	Creates a new linked list.
*
*	@return A pointer to a new, empty linked list. NULL if malloc() failed.
*/
linked_list *	new_ll();

/***
*	Deletes a linked list, deallocates all internal structures.
*	DOES NOT deallocate the data.
*
*	@param list 	The list.
*/
void 			delete_ll(linked_list * list);

/***
*	Adds a data point to the end of a list.
*
*	@param list 	The list.
*	@param data 	The data.
*	@return 		Pointer to the new list element, NULL if malloc() failed.
*/
list_element * 	add_to_ll(linked_list * list, void * data);

/***
*	Returns the data of a specific list element. Returns zero if the index is out of range.
*
*	@param list 	The list.
*	@param pos 		The index of the data point, starting at zero.
*	@return 		The data.
*/
void * 			get_data_from_ll(linked_list * list, int pos);

/***
*	Returns a specific list element. Returns zero if the index is out of range.
*	Using the list element allows to quickly get the next list element by using the next field.
*
*	@param list 	The list.
*	@param pos 		The index of the list element, starting at zero.
*	@return 		The list element. NULL if not found.
*/
list_element * 	get_element_from_ll(linked_list * list, int pos);

/***
*	Deletes a specific list element. Returns the pointer to the data of the
*	deleted list element.
*
*	@param list 	The list.
*	@param pos 		The index of the list element, starting at zero.
*	@return 		The data pointer of the deleted list element.
*/
void *			delete_element_ll(linked_list * list, int pos);

/***
*	Unlinks a specific list element without deallocating it. Returns the
*	pointer to the list element.
*
*	@param list 	The list.
*	@param pos 		The index of the list element, starting at zero.
*	@return 		The list element.
*/
list_element *	unlink_element_ll(linked_list * list, int pos);

/***
*	Returns 1 if the list is empty, returns 0 otherwise.
*
*	@param list 	The list.
*	@return 		1 if empty, otherwise 0.
*/
int				is_empty_ll(linked_list * list);

/***
*	Returns the length of the list
*
*	@param 	list 	The list.
*	@return 		The length of the list.
*/
int				length_ll(linked_list * list);



/*
TODO
void add_at_offset_ll(linked_list * list, void * data, int pos)
void overwrite_at_ll(linked_list * list, void * data, int pos)
*/
#endif
