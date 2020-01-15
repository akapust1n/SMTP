#ifndef STATES_H
#define STATES_H
enum ServerClientStates {
    SC_Greetings = 0,
    SC_INIT,
    SC_WAIT,
    SC_HELO,
    SC_MAIL_CREATED_NO_RECEPIENT,
    SC_RCPT,
    SC_WDATA,
    SC_RSET,
    SC_DELIVERING,
    SC_FINISH,
    SC_CLOSED,
    SC_Count
};

#endif
