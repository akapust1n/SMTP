#ifndef _CLIENT_RE_H_
#define _CLIENT_RE_H_

#include <profile.h>
#include <pcre2.h>
#include <stdbool.h>


#define SERVER_RESPONSE_CODE_CONNECT_OK "220 "
#define SERVER_RESPONSE_CODE_HELO_OK "250 "
#define SERVER_RESPONSE_CODE_HELO_OK_WITH_NAME "250-"
#define SERVER_RESPONSE_CODE_MAIL_OK "250 "
#define SERVER_RESPONSE_CODE_RCPT_OK "250 "
#define SERVER_RESPONSE_CODE_DATA_OK "354 "
#define SERVER_RESPONSE_CODE_DATA_RECEIVED_OK "250 "
#define SERVER_RESPONSE_CODE_QUIT_OK "221 "
#define SERVER_RESPONSE_CODE_EHLO_NOT_SUPPORTED "554 "

#define RE_SERVER_RESPONSE_CONNECT SERVER_RESPONSE_CODE_CONNECT_OK "(.*)$"
#define RE_SERVER_RESPONSE_HELLO   SERVER_RESPONSE_CODE_HELO_OK "(.*)$"
#define RE_SERVER_RESPONSE_HELLO_LINES SERVER_RESPONSE_CODE_HELO_OK_WITH_NAME "(.*)$"
#define RE_SERVER_RESPONSE_HELLO_LAST_LINE SERVER_RESPONSE_CODE_HELO_OK "(.*)$"
#define RE_SERVER_RESPONSE_MAILTO SERVER_RESPONSE_CODE_MAIL_OK "(.*)$"
#define RE_SERVER_RESPONSE_RCPTFROM SERVER_RESPONSE_CODE_RCPT_OK "(.*)$"
#define RE_SERVER_RESPONSE_READY_FOR_MESSAGE_BODY SERVER_RESPONSE_CODE_DATA_OK  "(.*)$"
#define RE_SERVER_RESPONSE_MESSAGE_BODY_RECIEVED SERVER_RESPONSE_CODE_DATA_RECEIVED_OK  "(.*)$"
#define RE_SERVER_RESPONSE_BYE SERVER_RESPONSE_CODE_QUIT_OK "(.*)$"
#define RE_SERVER_RESPONSE_EHLO_NOT_SUPPORTED  SERVER_RESPONSE_CODE_EHLO_NOT_SUPPORTED "(.*)$"
#define RE_SERVER_RESPONSE_ERROR  "^([0-9]{3}) (.*)$"

enum smtp_server_response
{
    SERVER_UNKNOWN_COMMAND,
    SERVER_CONNECT,
    SERVER_MAIL_OK,
    SERVER_RCPT_OK,
    SERVER_DATA_OK,
    SERVER_DATA_RECEIVED_OK,
    SERVER_QUIT_OK,
    SERVER_OK
};

struct smtp_server_response_re
{
    enum smtp_server_response response;
    const char *symbolic_name;
    PCRE2_SPTR pattern;
    pcre2_code *re;
};

struct smtp_server_response_match
{
    enum smtp_server_response response;
    pcre2_match_data *match_data;
};

int server_response_parser_init();
int server_response_parser_free();

bool server_response_parser_try_to_parse_message(char *message, struct smtp_server_response *regexp,
    struct smtp_server_response_match *match);

#endif
