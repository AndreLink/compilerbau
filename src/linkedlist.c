#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

//define DEBUGGGGGG to completely fill stdout with spam
//#define DEBUGGGGGG


/**
*   Unlinks a list element from the list. You better bloody keep a reference
*   to target, or it'll be lost in the depths of the heap!
*/
void INTERNAL_unlink_element_ll(linked_list * list, list_element * target)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: INTERNAL_unlink_element_ll(%p, %p);\n", list, target);
    #endif // DEBUGGGGGG
	if(target->prev)
	{
		target->prev->next = target->next;
	}
	else
	{
		list->first = target->next;
	}
	if(target->next)
	{
		target->next->prev = target->prev;
	}
	else
	{
		list->last = target->prev;
	}
}



linked_list * new_ll()
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: new_ll()\n");
    #endif // DEBUGGGGGG
	linked_list * ll = malloc(sizeof(linked_list));
	if(ll == 0) return 0;
	ll->first = 0;
	ll->last = 0;
	return ll;
}


void delete_ll(linked_list * list)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: delete_ll(%p)\n", list);
    #endif // DEBUGGGGGG
	while(list->first != 0)
	{
		delete_element_ll(list, 0);
	}
}

list_element * add_to_ll(linked_list * list, void * data)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: add_to_ll(%p, %p)\n", list, data);
    #endif // DEBUGGGGGG
	list_element * newElement = malloc(sizeof(list_element));
    if(newElement == 0) return 0;

	list_element * lastElement = list->last;

	newElement->prev = lastElement;
	newElement->next = 0;
	newElement->data = data;

	if(lastElement)
	{
		lastElement->next = newElement;
	}
	else
	{
		list->first = newElement;
	}
	list->last = newElement;
	return newElement;
}


void * delete_element_ll(linked_list * list, int pos)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: delete_element_ll(%p, %d)\n", list, pos);
    #endif // DEBUGGGGGG
	list_element * current = list->first;

	while (current != 0 )
	{
		if(pos == 0)
		{
			void * data = current->data;
			INTERNAL_unlink_element_ll(list, current);
			free(current);
			return data;
		}
		pos--;
		current = current->next;
	}
	return 0;
}


list_element * unlink_element_ll(linked_list * list, int pos)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: unlink_element_ll(%p, %d)\n", list, pos);
    #endif // DEBUGGGGGG
	list_element * current = list->first;

	while (current != 0 )
	{
		if(pos == 0)
		{
			INTERNAL_unlink_element_ll(list, current);
			return current;
		}
		pos--;
		current = current->next;
	}
	return 0;
}


list_element * get_element_from_ll(linked_list * list, int pos)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: get_element_from_ll(%p, %d)\n", list, pos);
    #endif // DEBUGGGGGG

	list_element * current = list->first;

	while (current != 0 )
	{
		if(pos == 0)
		{
			return current;
		}
		pos--;
		current = current->next;
	}
	return 0;
}


void * get_data_from_ll(linked_list * list, int pos)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: get_data_from_ll(%p, %d)\n", list, pos);
    #endif // DEBUGGGGGG

	list_element * current = list->first;

	while (current != 0 )
	{
		if(pos == 0)
		{
			return current->data;
		}
		pos--;
		current = current->next;
	}
	return 0;
}


int	length_ll(linked_list * list)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: length_ll(%p)\n", list);
    #endif // DEBUGGGGGG
	int length = 0;
	list_element * current = list->first;
	while(current != 0)
	{
		length++;
		current=current->next;
	}
	return length;
}

int	is_empty_ll(linked_list * list)
{
    #ifdef DEBUGGGGGG
    printf("## linkedlist.c: is_empty_ll(%p)\n", list);
    #endif // DEBUGGGGGG
	if(list->first)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


/*
TODO
void add_at_offset_ll(linked_list * list, void * data, int pos)
void overwrite_at_ll(linked_list * list, void * data, int pos)
*/
