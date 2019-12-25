#ifndef FSMINTERFACE_H
#define FSMINTERFACE_H
enum te_ss_state { //todo fix
};

//TODO
te_ss_state HANDLE_HELO(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_EHLO(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_MAIL(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_RCPT(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_DATA(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_GMSG(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_TMSG(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_QUIT(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_NOOP(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_VRFY(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_RSET(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
te_ss_state HANDLE_ERROR(int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz, te_ss_state nxt){};
#endif // FSMINTERFACE_H
