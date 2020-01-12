#ifndef CommonConsts_H
#define CommonConsts_H
#include <map>
#include <memory>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

enum ReturnCodes {
    GreetindCode = 220
};

constexpr int serverBufferSize = 1024;
constexpr const char code220[] = "220";
const int bufsize = 1024;
constexpr const char serverDomain[] = "myserver.com";
struct msg {
    std::vector<std::string> to;
    std::string from;
    std::string body;
    int body_length;
    int recepients_num;
};

struct Client {
    Client() {}
    Client(int fd, int buffer_size, int state, int needs_message)
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
using ClientsMap = std::map<int, Client>;
inline void mySend(const Client& client, const char msg[])
{
    int resultSend = send(client.fd, msg, strlen(msg), 0);
    if (resultSend > 0) {
        //init socket
        return;
    } else {
        //close socket
    }
}
#endif
