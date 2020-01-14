#ifndef SERVECLIENT_H
#define SERVECLIENT_H
#include "States.h"
#include "logger.h"
#include <Common.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

inline int handleHelo(Client& client, const sockaddr_in& address)
{
    if (client.state == SC_INIT) {
        std::string outputMsg = code220;
        if (mySend(client, outputMsg.c_str())) {
            client.state = SC_WAIT;
        }
        return -2;
    }
    return -1;
}

inline bool handleClient(int clientId, ClientsMap& clients)
{
    Client& client = clients[clientId];
    char bufferOutput[bufsize];
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(serverPort);
    //greetings
    if (client.state == SC_Greetings) {
        int size = sprintf(bufferOutput, "%s %s \r\n", code220, serverDomain);

        if (mySend(client, bufferOutput)) {
            client.state = SC_INIT;
            return true;
        } else {
            client.state = SC_CLOSED;
            return false;
        }
    }
    int recvBytes = recv(client.fd, client.buffer.get(), serverBufferSize, 0);
    if (recvBytes <= 5) {
        LOG_DEBUG("remote host closed socket %d\n", client.fd);
        client.state = SC_CLOSED;
        return false;
    } else {
        char cmd[5] = { client.buffer[0], client.buffer[1], client.buffer[2], client.buffer[3], '\0' };
        for (int i = 0; i < 5; ++i) {
            cmd[i] = static_cast<char>(toupper(cmd[i]));
        }
        int errCode = 0;
        if (!strcmp(cmd, "HELO")) {
            // начальное приветствие
            errCode = handleHelo(client, address);
        }
    }
}
#endif // SERVECLIENT_H
