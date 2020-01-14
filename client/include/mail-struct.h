#ifndef __MAIL_STRUCT_H__
#define __MAIL_STRUCT_H__

#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAIL_FROM_STRING "From: "
#define MAIL_TO_STRING "To: "

#define MAIL_CHUNK_SIZE 256

struct mail_struct
{
	char *from;
	char *to;
	char *message_body;
};

int load_file_to_mail_struct(const char* path, struct mail_struct *out_mail_struct)
int free_mail_struct(struct mail_struct *mail_struct);

#endif
