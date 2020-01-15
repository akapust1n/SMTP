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
constexpr const int serverPort = 10000;

constexpr int serverBufferSize = 1024;
constexpr const char code220[] = "220";
constexpr const char code250[] = "250";
constexpr const char code451[] = "451";
constexpr const char code354[] = "354";
constexpr const char code221[] = "221";
const int bufsize = 1024;
constexpr const char serverDomain[] = "myserver.com";
struct msg {
    std::vector<std::string> to;
    std::string from;
    std::string body;
};

constexpr const int maxRecepients = 5;

struct Client {
    Client() {}
    Client(int fd, int buffer_size, int state, int needs_message)
        : fd(fd)
        , state(state)
        , buffer_offset(0)
        , message(nullptr)
        , flag(1)
    {

        buffer.reset(new char[1024]);
        if (needs_message) {
            message.reset(new msg);
        }
    }
    int fd;
    std::shared_ptr<char[]> buffer;
    int buffer_offset;
    int input_message;
    int state;
    int flag;
    std::shared_ptr<msg> message;
};
using ClientsMap = std::map<int, Client>;
inline bool mySend(const Client& client, const char msg[])
{
    int resultSend = send(client.fd, msg, strlen(msg), 0);
    return resultSend > 0;
}

#endif
