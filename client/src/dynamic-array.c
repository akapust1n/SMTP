/** \file dynamic-array.c
 *  \brief Функции для работы с динамическими массивами.
 *
 *  В этом файле описаны функции для работы с динамическими массивами.
 */

#include "dynamic-array.h"
#include "string.h"
#include "stdlib.h"


int dynamic_array_create(uint32_t element_size, uint32_t initial_size, struct dynamic_array* array)
{
	if (array != NULL)
	{
		perror("Non NULL-pointer passed to dynamic_array_create");
		return -1;
	}
	array = (struct dynamic_array*)malloc(sizeof(struct dynamic_array));
	array->data = (char*)malloc(element_size * initial_size);
	array->current_number_of_elements = 0;
	array->max_number_of_elements = initial_size;
	array->element_size = element_size;
	return 0;
}

int dynamic_array_get_item(uint32_t index, const struct dynamic_array* array, char** item)
{
	uint64_t offset;

	if (item == NULL)
	{
		perror("NULL-pointer passed as target item to dynamic_array_get_item");
		return -1;
	}

	if (array == NULL)
	{
		perror("NULL-pointer passed as target array to dynamic_array_get_item");
		*item = NULL;
		return -2;
	}
	
	if (index >= array->number_of_elements)
	{
		perror("Requested index is out of bonds");
		*item = NULL;
		return -3;
	}
	offset = array->element_size * index;
	
	*item = array->data + offset;

	return 0;
}

int dynamic_array_put_item(struct dynamic_array* array, const char* item)
{	
	if (array == NULL)
	{
		perror("NULL-pointer passed as target array to dynamic_array_put_item");
		return -1;
	}

	if (array->current_number_of_elements == array->max_number_of_elements)
	{
		uint64_t current_size = array->current_number_of_elements * array->element_size;
		uint64_t new_size = current_size * 2;
		
		if (current_size >=  MAX_DYNAMIC_ARRAY_SIZE)
		{
			perror("Impossible to insert new item into the array as the max size of the array reached");
			return -2;
		}

		if (new_size > MAX_DYNAMIC_ARRAY_SIZE)
			new_size = MAX_DYNAMIC_ARRAY_SIZE;

		char *result = realloc(array->data, new_size);
		if (result == NULL)
		{
			perror("Realloc failed");
			return -3;
		}
		array->data = result;
		array->max_number_of_elements = new_size / array->element_size;
	}
	
	if (array->current_number_of_elements == array->max_number_of_elements)
	{
		uint64_t offset = array->current_number_of_elements * array->element_size;
		memcpy(array->data + offset, item, array->element_size);
		array->current_number_of_elements++;
		return 0;
	}

	perror("Unable to insert new item to the array");
	return -4;
}

int dynamic_array_pop_item(const dynamic_array* array, char** item)
{
	if (item == NULL || array == NULL)
	{
		perror("NULL-ponter as an argument to dynamic_array_pop_item");
		return -1;
	}

	if (array->current_number_of_elements > 0)
	{
		if (*item == NULL)
		{
			*item = (char*)malloc(array->element_size);
			if (*item == NULL)
			{
				perror("Malloc failed");
				return -2;
			}
		}
		
		uint64_t offset = (array->current_number_of_elements-1) * array->element_size;
		memcpy(*item, array->data + offset, array->element_size);
		array->current_number_of_elements--;
	}
	else
	{
		*item = NULL;
	}
	return 0;
}

