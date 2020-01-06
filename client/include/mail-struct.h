#ifndef __MAIL_STRUCT_H__
#define __MAIL_STRUCT_H__

struct mail_file_descriptor
{
	char *domain;
	FILE *file_ptr;
};

struct mail_struct
{
	char *from;
	char *to;
	char *message_body;
};

#endif
