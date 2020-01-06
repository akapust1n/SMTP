#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

struct dynamic_array
{
	uint32_t current_number_of_items;
	uint32_t max_number_of_items;
	uint32_t element_size;
	char* data;
};


int dynamic_array_create(uint32_t element_size, uint32_t initial_size, struct dynamic_array* array);
int dynamic_array_free(struct dynamic_array* array);
int dynamic_array_get_item(uint32_t index, const struct dynamic_array* array, char** item);
int dynamic_array_put_item(struct dynamic_array* array, const char* item);
int dynamic_array_pop_item(const dynamic_array* array, char* item);

#endif

