#include <stdio.h>
#include <string.h>
#include "client-re.h"


bool server_response_parser_try_to_parse_message(char *message, struct smtp_server_response_re *regexp,
                                                 struct smtp_server_response_match *match);


static struct smtp_server_response_re server_responses[] =
{
    (struct smtp_server_response_re){SERVER_CONNECT, "CONNECT RESPONSE", (PCRE2_SPTR)RE_SERVER_RESPONSE_CONNECT, NULL},
    (struct smtp_server_response_re){SERVER_HELLO_OK, "HELLO RESPONSE", (PCRE2_SPTR)RE_SERVER_RESPONSE_HELLO, NULL},
    (struct smtp_server_response_re){SERVER_HELLO_OK_MULTILINE, "HELLO MULTILINE RESPONSE",
                                     (PCRE2_SPTR)RE_SERVER_RESPONSE_HELLO_LINES, NULL},
    (struct smtp_server_response_re){SERVER_MAIL_OK, "MAIL TO RESPONSE", (PCRE2_SPTR)RE_SERVER_RESPONSE_MAILTO, NULL},
    (struct smtp_server_response_re){SERVER_RCPT_OK, "RCPT FROM RESPONSE",
                                     (PCRE2_SPTR)RE_SERVER_RESPONSE_RCPTFROM, NULL},
    (struct smtp_server_response_re){SERVER_DATA_OK, "DATA RESPONSE",
                                     (PCRE2_SPTR)RE_SERVER_RESPONSE_READY_FOR_MESSAGE_BODY, NULL},
    (struct smtp_server_response_re){SERVER_DATA_RECEIVED_OK, "DATA RECEIVED RESPONSE",
                                     (PCRE2_SPTR)RE_SERVER_RESPONSE_MESSAGE_BODY_RECIEVED, NULL},
    (struct smtp_server_response_re){SERVER_QUIT_OK, "QUIT RESPONSE", (PCRE2_SPTR)RE_SERVER_RESPONSE_BYE, NULL},
    (struct smtp_server_response_re){SERVER_EHLO_NOT_SUPPORTED, "EHLO NOT SUPPORTED RESPONSE",
                                     (PCRE2_SPTR)RE_SERVER_RESPONSE_EHLO_NOT_SUPPORTED, NULL},
    (struct smtp_server_response_re){SERVER_RSET_OK, "RSET RESPONSE", (PCRE2_SPTR)RE_SERVER_RESPONSE_RSET, NULL},
    (struct smtp_server_response_re){SERVER_ERROR, "ERROR RESPONSE", (PCRE2_SPTR)RE_SERVER_RESPONSE_ERROR, NULL}
};



bool server_response_parser_try_to_parse_message(char *message, struct smtp_server_response_re *regexp,
                                                 struct smtp_server_response_match *match)
{
    if ((message == NULL) || (regexp == NULL) || (match == NULL))
    {
        perror("Passed NULL-ptr to server_response_parser_try_to_parse_message()");
        return false;
    }

    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(regexp->re, NULL);

    int result = pcre2_match(regexp->re, (PCRE2_SPTR)message, strlen(message),
            0, 0, match_data, NULL);

    if (result < 0)
    {
        if (result != PCRE2_ERROR_NOMATCH)
        {
            char buffer[512];
            pcre2_get_error_message(result, (PCRE2_UCHAR*)buffer, 512);
            printf("PCRE2 ERROR: %s\n", buffer);
        }
        pcre2_match_data_free(match_data);
        match->match_data = NULL;
        match->response = SERVER_UNKNOWN_RESPONSE;
        return false;
    }
    match->match_data = match_data;
    match->response = regexp->response;

    return true;
}

struct smtp_server_response_match server_response_parse_message(char *message)
{
    struct smtp_server_response_match result =
    {
            .response = SERVER_UNKNOWN_RESPONSE,
            .match_data = NULL
    };

    size_t n = sizeof(server_responses) / sizeof(server_responses[0]);

    for (size_t i = 0; i < n; i++)
    {
        struct smtp_server_response_re *cmd = server_responses + i;
        bool matched = server_response_parser_try_to_parse_message(message, cmd, &result);
        if (matched)
        {
            return result;
        }
    }

    return result;
}


int server_response_parser_init()
{
    size_t n = sizeof(server_responses) / sizeof(server_responses[0]);

    for (size_t i = 0; i < n; i++)
    {
        struct smtp_server_response_re *cmd = server_responses + i;
        size_t error_offset = 0;
        int error_number = 0;

        cmd->re = pcre2_compile(cmd->pattern, PCRE2_ZERO_TERMINATED,
                                0, &error_number, &error_offset, NULL);

        if (cmd->re == NULL)
        {
            PCRE2_UCHAR buffer[256];
            pcre2_get_error_message(error_number, buffer, sizeof(buffer));
            printf("PCRE2 compilation of string %s failed at offset %d: %s\n",
                   cmd->pattern, (int)error_offset, buffer);
        }
    }
    return 0;
}

int server_response_parser_free()
{
    size_t n = sizeof(server_responses) / sizeof(server_responses[0]);

    for (size_t i = 0; i < n; i++)
    {
        struct smtp_server_response_re *cmd = server_responses + i;
        pcre2_code_free(cmd->re);
    }
}
