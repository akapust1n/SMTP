#include "outgoing-mail-struct.h"


unsigned long crc32(const void *data, uint32_t data_size)
{
	uint32_t crc32_value = 0;
  
  	for (uint32_t i = 0;  i < data_size;  i++)
  	{
      		crc32_value = crc32_table[(crc32_value ^ data[i]) & 0xff] ^ (crc32_value >> 8);
	}

	return crc32_value;
}

uint32_t string_hash_function(const void *data, uint32_t data_size)
{
	uint32_t key = crc32(data, data_size);

	/* Robert Jenkins' 32 bit Mix Function */
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

	/* Knuth's Multiplicative Method */
	key = (key >> 3) * 2654435761;

	return key;
}

bool string_compare_function(const void *first, const void *second, uint32_t size)
{
	return strncmp((const char *)first, (const char *)second, size) == 0;
}

uint32_t outgoing_mail_create_node(struct hashtable_node_list *node_list, void *key, uint32_t key_size, void *data, uint32_t data_size)
{
	if (node_list->list == NULL)
	{
		node_list->list = (struct hashtable_node*)malloc(sizeof(hashtable_node));
		node_list->list->key = (void *)malloc(key_size);
		node_list->list->value = (void)malloc(data_size);
		memcpy(node_list->list->key, key, key_size);
		memcpy(node_list->list->data, data, data_size);
		node_list->list->next = NULL;
	}
	else
	{
		struct hashtable_node *new_node = (struct hashtable_node*)malloc(sizeof(hashtable_node));
		new_node->key = (void *)malloc(key_size);
		new_node->value = (void)malloc(data_size);
		memcpy(new_node->key, key, key_size);
		memcpy(new_node->list->data, data, data_size);
		new_node->next = node_list->list;
		node_list->list = new_node;
	}
	return 0;
}

uint32_t outgoing_mail_free_node(struct hashtable_node_list *node_list, void *key, uint32_t key_size)
{
	struct hashtable_node *current_node = node_list->list;
	struct 	hashtable_node *previous_node = NULL;
	
	while(current_node != NULL)
	{
		if (string_compare_function(key, current_node->key, key_size))
		{
			if (previous_node == NULL)
			{
				node_list->list = node_list->list->next;
				free(current_node->key);
				free(current_node->value);
				free(current_node);
			}
			else
			{
				previous_node->next = current_node->next;
				free(current_node->key);
				free(current_node->value);
				free(current_node);
			}
			return 0;
		}
		previous_node = current_node;
		current_node = current_node->next;
	}
}


struct hashtable *create_outgoing_mail_dictionary_from_directory(const char *path)
{
	struct hashtable *hashtable = hashtable_create(&string_hash_function, &string_compare_function,
		&outgoing_mail_create_node, &outgoing_mail_free_node);

	struct linked_list *dir_listing = NULL;
	get_directory_listing(path, dir_listing);

	
	for (uint32_t index = 0; index < dir_listing->length; index++)
	{
		char *file_path = (char *)linked_list_get(dir_listing, index);
		struct outgoing_mail_struct *oms = create_outgoing_mail_struct_from_file(file_path);
		char *domain = extract_domain_from_mail_file(file_path);
		hashtable_put(hashtable, domain, strlen(domain), )
	}

}

void init_outgoing_mail_directory(const char *path)
{

}

struct linked_list *get_mail_struct_by_domain(const char *domain)
{

}

