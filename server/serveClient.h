#ifndef SERVECLIENT_H
#define SERVECLIENT_H
#include <memory>
#include <string>
#include <vector>

struct msg {
    std::vector<std::string> to;
    std::string from;
    std::string body;
    int body_length;
    int recepients_num;
};
struct client_socket {
    client_socket(int fd, int buffer_size, int state, int needs_message)
        : fd(fd)
        , state(state)
        , buffer_offset(0)
        , message(nullptr)
        , flag(1)
    {
        if (buffer_size > 0) {
            buffer.resize(buffer_size);
        }

        if (needs_message) {
            message.reset(new msg);
            message->recepients_num = 0;
        }
    }
    int fd;
    std::string buffer;
    int buffer_offset;
    int input_message;
    int state;
    int flag;
    std::shared_ptr<msg> message;
};
struct client_socket init_client_socket(int fd, int buffer_size, int state, int max_recepients, int needs_message)
{
    struct client_socket result;

    result.fd = fd;
    if (buffer_size > 0) {
        result.buffer.resize(buffer_size);
    }
    result.state = state;
    result.buffer_offset = 0;
    result.message = NULL;
    result.flag = 1;
    if (needs_message) {
        result.message.reset(new msg);
        result.message->recepients_num = 0;
    }
    return result;
}
#endif // SERVECLIENT_H
