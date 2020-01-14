#ifndef STATES_H
#define STATES_H
enum ServerClientStates {
    SC_Greetings = 0,
    SC_INIT,
    SC_WAIT,
    SC_HELO,
    SC_MAIL,
    SC_RCPT,
    SC_DATA,
    SC_GMSG,
    SC_TMSG,
    SC_INVALID,
    SC_DONE,
    SC_READY,
    SC_ERROR,
    SC_CLOSED,
    SC_Count
};

#endif
