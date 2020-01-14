#include "mail-struct.h"


int load_file_to_mail_struct(const char* path, struct mail_struct *out_mail_struct)
{
    FILE *file = fopen(path, "r");
    char *buffer = (char *) malloc(MAIL_CHUNK_SIZE);
    char *from_string_ptr = NULL;
    char *to_string_ptr = NULL;
    size_t total_len = 0;
    while (fgets(buffer, MAIL_CHUNK_SIZE, file) != NULL) {
        if (from_string_ptr == NULL) {
            from_string_ptr = strstr(buffer, MAIL_FROM_STRING);
        }
        if (to_string_ptr == NULL) {
            to_string_ptr = strstr(buffer, MAIL_TO_STRING);
        }
        total_len += strlen(buffer);
    }
    out_mail_struct->from = malloc(strlen(from_string_ptr + strlen(MAIL_FROM_STRING)));
    strcpy(out_mail_struct->from, from_string_ptr + strlen(MAIL_FROM_STRING));
    out_mail_struct->to = malloc(strlen(to_string_ptr + strlen(MAIL_TO_STRING)));
    strcpy(out_mail_struct->to, to_string_ptr + strlen(MAIL_TO_STRING));
    out_mail_struct->message_body = malloc(total_len + 1);
    rewind(file);

    size_t offset = 0;

    while (fgets(buffer, MAIL_CHUNK_SIZE, file) != NULL)
    {
        strcpy(out_mail_struct->message_body + offset, buffer);
        offset += strlen(buffer);
    }

    fclose(file);
    free(buffer);

    return 0;
}

int free_mail_struct(struct mail_struct *mail_struct)
{
    free(mail_struct->to);
    free(mail_struct->from);
    free(mail_struct->message_body);
}
