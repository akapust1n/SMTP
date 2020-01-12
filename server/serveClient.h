#ifndef SERVECLIENT_H
#define SERVECLIENT_H
#include <Common.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

inline bool handleClient(int clientId, ClientsMap& clients)
{
    Client& client = clients[clientId];
    //greetings
    if (client.state == -1) //todo add enum
    {
        char buffer[bufsize];
        int size = sprintf(buffer, "%s %s \r\n", code220, serverDomain);

        mySend(client, buffer);
    }
}
#endif // SERVECLIENT_H
