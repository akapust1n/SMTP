#ifndef __OUTGOING_MAIL_STRUCT_H__
#define __OUTGOING_MAIL_STRUCT_H__

#include "mail-struct.h"
#include "hashtable.h"
#include "linked-list.h"
#include "path-utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAIL_READ_BUFFER_SIZE 120

int outgoing_mail_dictionary_add_files_from_directory(struct hashtable *hashtable, const char *path);
struct hashtable *outgoing_mail_dictionary_create();
uint32_t outgoing_mail_free_node(struct hashtable_node_list *node_list, void *key, uint32_t key_size);

#endif

