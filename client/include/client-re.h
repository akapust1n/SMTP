#ifndef _CLIENT_RE_H_
#define _CLIENT_RE_H_

#define PCRE2_CODE_UNIT_WIDTH 8

#include <profile.h>
#include <pcre2.h>
#include <stdbool.h>


#define SERVER_RESPONSE_CODE_CONNECT_OK "220 "
#define SERVER_RESPONSE_CODE_OK "250 "
#define SERVER_RESPONSE_CODE_HELO_OK_MULTILINE "250-"
#define SERVER_RESPONSE_CODE_DATA_OK "354"
#define SERVER_RESPONSE_CODE_QUIT_OK "221"
#define SERVER_RESPONSE_CODE_EHLO_NOT_SUPPORTED "554"

#define RE_SERVER_RESPONSE_CONNECT SERVER_RESPONSE_CODE_CONNECT_OK "(.*)$"
#define RE_SERVER_RESPONSE_EHLO_LINES SERVER_RESPONSE_CODE_HELO_OK_MULTILINE "(.*)$"
#define RE_SERVER_RESPONSE_READY_FOR_MESSAGE_BODY SERVER_RESPONSE_CODE_DATA_OK  "(.*)$"
#define RE_SERVER_RESPONSE_BYE SERVER_RESPONSE_CODE_QUIT_OK "(.*)$"
#define RE_SERVER_RESPONSE_OK SERVER_RESPONSE_CODE_OK "(.*)$"
#define RE_SERVER_RESPONSE_EHLO_NOT_SUPPORTED  SERVER_RESPONSE_CODE_EHLO_NOT_SUPPORTED "(.*)$"
#define RE_SERVER_RESPONSE_ERROR  "^([0-9]{3}) (.*)$"

enum smtp_server_response
{
    SERVER_UNKNOWN_RESPONSE,
    SERVER_CONNECT,
    SERVER_OK,
    SERVER_EHLO_OK_MULTILINE,
    SERVER_DATA_OK,
    SERVER_QUIT_OK,
    SERVER_EHLO_NOT_SUPPORTED,
    SERVER_ERROR
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

struct smtp_server_response_match server_response_parse_message(char *message, uint32_t last_match_position);

#endif
