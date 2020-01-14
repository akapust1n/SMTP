#include "serveClient.h"
std::string getMail(const std::string& msg)
{
    unsigned first = msg.find('<');
    unsigned last = msg.find('>');
    return msg.substr(first, last - first);
}
int handleHelo(Client& client, const sockaddr_in& address)
{
    if (client.state == SC_INIT) {
        std::string outputMsg = code250;
        if (mySend(client, outputMsg.c_str())) {
            client.state = SC_WAIT;
            return 0;
        }
        return -2;
    }
    return -1;
}
int handleEhlo(Client& client, const sockaddr_in& address)
{
    return handleHelo(client, address);
}

int handleMail(Client& client, const sockaddr_in& address)
{
    if (client.state == SC_WAIT) {
        std::string from = getMail(client.buffer.get());
        if (from.empty()) //todo validation
            return -3;
        else
            client.message->from = from;
        std::string outputMsg = code250;
        if (mySend(client, outputMsg.c_str())) {
            client.state = SC_MAIL_CREATED_NO_RECEPIENT;
            return 0;
        }
        return -2;
    }
    return -1;
}

int handleRCPT(Client& client, const sockaddr_in& address)
{
    if (client.state == SC_RCPT || client.state == SC_MAIL_CREATED_NO_RECEPIENT) {
        std::string outputMsg = code451;
        if (client.message->to.size() >= maxRecepients) {
            if (mySend(client, outputMsg.c_str())) {
                return 0;
            }
        } else {
            std::string to = getMail(client.buffer.get());
            if (to.empty()) //todo validation
                return -3;
            else
                client.message->to.push_back(to);
            outputMsg = code250;
            if (mySend(client, outputMsg.c_str())) {
                client.state = SC_RCPT;
                return 0;
            }
        }
    }
    return -1;
}
