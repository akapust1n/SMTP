#ifndef __OUTGOING_MAIL_STRUCT_H__
#define __OUTGOING_MAIL_STRUCT_H__

#include "mail-struct.h"
#include "hashtable.h"
#include "linked-list.h"
#include "path-utils.h"

enum mail_state
{
	waiting_to_be_staged,
	staged_to_send,
	sending,
	successfuly_sent,
	error
};

struct outgoing_mail_struct
{
	struct mail_file_descriptor* mail_fd;
	enum mail_state state;
	unsigned int current_send_attempt_count;
};

struct hashtable *create_outgoing_mail_dictionary_from_directory(const char *path);
void init_outgoing_mail_directory(const char *path);
struct linked_list *get_mail_struct_by_domain(const char *domain);

#endif

