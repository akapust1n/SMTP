#include "serveClient.h"
std::string getMail(const std::string& msg)
{
    unsigned first = msg.find('<');
    unsigned last = msg.find('>');
    return msg.substr(first, last - first);
}
int handleHelo(Client& client)
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
int handleEhlo(Client& client)
{
    return handleHelo(client);
}

int handleMail(Client& client)
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

int handleRCPT(Client& client)
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
int handleData(Client& client)
{
    if (client.state == SC_RCPT) {
        std::string outputMsg = code354;

        if (mySend(client, outputMsg.c_str())) {
            client.state = SC_WDATA;
            return 0;
        }
    }
    return -1;
}

int handleRSET(Client& client)
{
    if ((client.state == SC_WAIT) || (client.state == SC_MAIL_CREATED_NO_RECEPIENT)
        || (client.state == SC_RCPT) || (client.state == SC_DELIVERING)) {
        client.message->to.clear();
        std::string outputMsg = code250;
        if (mySend(client, outputMsg.c_str())) {
            client.state = SC_WAIT;
            return 0;
        }
    }
    return -1;
}

int handleQuit(Client& client)
{
    std::string outputMsg = code221;
    mySend(client, outputMsg.c_str());
    return -1; // expected
}

int handleVRFY(Client& client)
{
    std::string outputMsg = code451;
    mySend(client, outputMsg.c_str());
    return 0; // expected
}

int handleText(Client& client)
{
    client.message->body.append(client.buffer.get());
    std::string msg = client.buffer.get();
    if (msg.find("\r\n.\r\n") != std::string::npos) {
        client.state = SC_DELIVERING;
    } else {
        std::string outputMsg = code250;
        mySend(client, outputMsg.c_str());
        client.state = SC_FINISH;
    }
    return 0;
}
