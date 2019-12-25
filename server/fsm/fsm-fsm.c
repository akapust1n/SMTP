/*   -*- buffer-read-only: t -*- vi: set ro:
 *
 *  DO NOT EDIT THIS FILE   (fsm-fsm.c)
 *
 *  It has been AutoGen-ed
 *  From the definitions    fsm.def
 *  and the template file   fsm
 *
 *  Automated Finite State Machine
 *
 *  Copyright (C) 1992-2016 Bruce Korb - all rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Bruce Korb'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * AutoFSM IS PROVIDED BY Bruce Korb ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Bruce Korb OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define DEFINE_FSM
#include "fsm-fsm.h"
#include <stdio.h>

/*
 *  Do not make changes to this file, except between the START/END
 *  comments, or it will be removed the next time it is generated.
 */
/* START === USER HEADERS === DO NOT CHANGE THIS COMMENT */
#include "../fsmInterface.h"
#include <stdlib.h>
/* END   === USER HEADERS === DO NOT CHANGE THIS COMMENT */

#ifndef NULL
#  define NULL 0
#endif

/**
 *  Enumeration of the valid transition types
 *  Some transition types may be common to several transitions.
 */
typedef enum {
    SS_TR_DATA,
    SS_TR_EHLO,
    SS_TR_ERROR,
    SS_TR_GMSG,
    SS_TR_HELO,
    SS_TR_INVALID,
    SS_TR_MAIL,
    SS_TR_NOP_,
    SS_TR_QUIT,
    SS_TR_RCPT,
    SS_TR_RSET,
    SS_TR_TMSG,
    SS_TR_VRFY
} te_ss_trans;
#define SS_TRANSITION_CT  13

/**
 *  State transition handling map.  Map the state enumeration and the event
 *  enumeration to the new state and the transition enumeration code (in that
 *  order).  It is indexed by first the current state and then the event code.
 */
typedef struct ss_transition t_ss_transition;
struct ss_transition {
    te_ss_state  next_state;
    te_ss_trans  transition;
};
static const t_ss_transition
ss_trans_table[ SS_STATE_CT ][ SS_EVENT_CT ] = {

  /* STATE 0:  SS_ST_INIT */
  { { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  EHLO */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  HELO */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  MAIL */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  RCPT */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  DATA */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  RSET */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  VRFY */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  NOP_ */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  QUIT */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  TMSG */
    { SS_ST_INVALID, SS_TR_INVALID },               /* EVT:  GMSG */
    { SS_ST_INVALID, SS_TR_INVALID }                /* EVT:  ERROR */
  },


  /* STATE 1:  SS_ST_HELO */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_MAIL, SS_TR_MAIL },                     /* EVT:  MAIL */
    { SS_ST_ERROR, SS_TR_RCPT },                    /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_HELO, SS_TR_VRFY },                     /* EVT:  VRFY */
    { SS_ST_HELO, SS_TR_NOP_ },                     /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_ERROR, SS_TR_TMSG },                    /* EVT:  TMSG */
    { SS_ST_ERROR, SS_TR_GMSG },                    /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 2:  SS_ST_MAIL */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_RCPT, SS_TR_RCPT },                     /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_MAIL, SS_TR_VRFY },                     /* EVT:  VRFY */
    { SS_ST_MAIL, SS_TR_NOP_ },                     /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_ERROR, SS_TR_TMSG },                    /* EVT:  TMSG */
    { SS_ST_ERROR, SS_TR_GMSG },                    /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 3:  SS_ST_RCPT */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_RCPT, SS_TR_RCPT },                     /* EVT:  RCPT */
    { SS_ST_DATA, SS_TR_DATA },                     /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_RCPT, SS_TR_VRFY },                     /* EVT:  VRFY */
    { SS_ST_RCPT, SS_TR_NOP_ },                     /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_ERROR, SS_TR_TMSG },                    /* EVT:  TMSG */
    { SS_ST_ERROR, SS_TR_GMSG },                    /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 4:  SS_ST_DATA */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_ERROR, SS_TR_RCPT },                    /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_DATA, SS_TR_VRFY },                     /* EVT:  VRFY */
    { SS_ST_DATA, SS_TR_NOP_ },                     /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_TMSG, SS_TR_TMSG },                     /* EVT:  TMSG */
    { SS_ST_GMSG, SS_TR_GMSG },                     /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 5:  SS_ST_GMSG */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_ERROR, SS_TR_RCPT },                    /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_GMSG, SS_TR_VRFY },                     /* EVT:  VRFY */
    { SS_ST_GMSG, SS_TR_NOP_ },                     /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_TMSG, SS_TR_TMSG },                     /* EVT:  TMSG */
    { SS_ST_GMSG, SS_TR_GMSG },                     /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 6:  SS_ST_TMSG */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_ERROR, SS_TR_RCPT },                    /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_TMSG, SS_TR_VRFY },                     /* EVT:  VRFY */
    { SS_ST_TMSG, SS_TR_NOP_ },                     /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_ERROR, SS_TR_TMSG },                    /* EVT:  TMSG */
    { SS_ST_ERROR, SS_TR_GMSG },                    /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 7:  SS_ST_READY */
  { { SS_ST_HELO, SS_TR_EHLO },                     /* EVT:  EHLO */
    { SS_ST_HELO, SS_TR_HELO },                     /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_ERROR, SS_TR_RCPT },                    /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_READY, SS_TR_VRFY },                    /* EVT:  VRFY */
    { SS_ST_READY, SS_TR_NOP_ },                    /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_ERROR, SS_TR_TMSG },                    /* EVT:  TMSG */
    { SS_ST_ERROR, SS_TR_GMSG },                    /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  },


  /* STATE 8:  SS_ST_ERROR */
  { { SS_ST_ERROR, SS_TR_EHLO },                    /* EVT:  EHLO */
    { SS_ST_ERROR, SS_TR_HELO },                    /* EVT:  HELO */
    { SS_ST_ERROR, SS_TR_MAIL },                    /* EVT:  MAIL */
    { SS_ST_ERROR, SS_TR_RCPT },                    /* EVT:  RCPT */
    { SS_ST_ERROR, SS_TR_DATA },                    /* EVT:  DATA */
    { SS_ST_READY, SS_TR_RSET },                    /* EVT:  RSET */
    { SS_ST_ERROR, SS_TR_VRFY },                    /* EVT:  VRFY */
    { SS_ST_ERROR, SS_TR_NOP_ },                    /* EVT:  NOP_ */
    { SS_ST_DONE, SS_TR_QUIT },                     /* EVT:  QUIT */
    { SS_ST_ERROR, SS_TR_TMSG },                    /* EVT:  TMSG */
    { SS_ST_ERROR, SS_TR_GMSG },                    /* EVT:  GMSG */
    { SS_ST_ERROR, SS_TR_ERROR }                    /* EVT:  ERROR */
  }
};


#define SsFsmErr_off     19
#define SsEvInvalid_off  75
#define SsStInit_off     83


static char const zSsStrings[155] =
/*     0 */ "** OUT-OF-RANGE **\0"
/*    19 */ "FSM Error:  in state %d (%s), event %d (%s) is invalid\n\0"
/*    75 */ "invalid\0"
/*    83 */ "init\0"
/*    88 */ "helo\0"
/*    93 */ "mail\0"
/*    98 */ "rcpt\0"
/*   103 */ "data\0"
/*   108 */ "gmsg\0"
/*   113 */ "tmsg\0"
/*   118 */ "ready\0"
/*   124 */ "error\0"
/*   130 */ "ehlo\0"
/*   135 */ "rset\0"
/*   140 */ "vrfy\0"
/*   145 */ "nop_\0"
/*   150 */ "quit";

static const size_t aszSsStates[9] = {
    83,  88,  93,  98,  103, 108, 113, 118, 124 };

static const size_t aszSsEvents[13] = {
    130, 88,  93,  98,  103, 135, 140, 145, 150, 113, 108, 124, 75 };


#define SS_EVT_NAME(t)   ( (((unsigned)(t)) >= 13) \
    ? zSsStrings : zSsStrings + aszSsEvents[t])

#define SS_STATE_NAME(s) ( (((unsigned)(s)) >= 9) \
    ? zSsStrings : zSsStrings + aszSsStates[s])

#ifndef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

static int ss_invalid_transition( te_ss_state st, te_ss_event evt );

/* * * * * * * * * THE CODE STARTS HERE * * * * * * * */
/**
 *  Print out an invalid transition message and return EXIT_FAILURE
 */
static int
ss_invalid_transition( te_ss_state st, te_ss_event evt )
{
    /* START == INVALID TRANS MSG == DO NOT CHANGE THIS COMMENT */
    char const * fmt = zSsStrings + SsFsmErr_off;
    fprintf( stderr, fmt, st, SS_STATE_NAME(st), evt, SS_EVT_NAME(evt));
    /* END   == INVALID TRANS MSG == DO NOT CHANGE THIS COMMENT */

    return EXIT_FAILURE;
}

/**
 *  Step the FSM.  Returns the resulting state.  If the current state is
 *  SS_ST_DONE or SS_ST_INVALID, it resets to
 *  SS_ST_INIT and returns SS_ST_INIT.
 */
te_ss_state
ss_step(
    te_ss_state ss_state,
    te_ss_event trans_evt,
    int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz )
{
    te_ss_state nxtSt;
    te_ss_trans trans;

    if ((unsigned)ss_state >= SS_ST_INVALID) {
        return SS_ST_INIT;
    }

#ifndef __COVERITY__
    if (trans_evt >= SS_EV_INVALID) {
        nxtSt = SS_ST_INVALID;
        trans = SS_TR_INVALID;
    } else
#endif /* __COVERITY__ */
    {
        const t_ss_transition * ttbl =
            ss_trans_table[ ss_state ] + trans_evt;
        nxtSt = ttbl->next_state;
        trans = ttbl->transition;
    }


    switch (trans) {
    case SS_TR_DATA:
        /* START == DATA == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_DATA(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == DATA == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_EHLO:
        /* START == EHLO == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_EHLO(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == EHLO == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_ERROR:
        /* START == ERROR == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_ERROR(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == ERROR == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_GMSG:
        /* START == GMSG == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_GMSG(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == GMSG == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_HELO:
        /* START == HELO == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_HELO(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == HELO == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_INVALID:
        /* START == INVALID == DO NOT CHANGE THIS COMMENT */
        exit(ss_invalid_transition(ss_state, trans_evt));
        /* END   == INVALID == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_MAIL:
        /* START == MAIL == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_MAIL(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == MAIL == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_NOP_:
        /* START == NOP_ == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_NOOP(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == NOP_ == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_QUIT:
        /* START == QUIT == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_QUIT(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == QUIT == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_RCPT:
        /* START == RCPT == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_RCPT(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == RCPT == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_RSET:
        /* START == RSET == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_RSET(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == RSET == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_TMSG:
        /* START == TMSG == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_TMSG(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == TMSG == DO NOT CHANGE THIS COMMENT */
        break;


    case SS_TR_VRFY:
        /* START == VRFY == DO NOT CHANGE THIS COMMENT */
        nxtSt = HANDLE_VRFY(fd, matchdata, matchdatal, recv_buf, recv_sz, nxtSt);
        /* END   == VRFY == DO NOT CHANGE THIS COMMENT */
        break;


    default:
        /* START == BROKEN MACHINE == DO NOT CHANGE THIS COMMENT */
        exit(ss_invalid_transition(ss_state, trans_evt));
        /* END   == BROKEN MACHINE == DO NOT CHANGE THIS COMMENT */
    }


    /* START == FINISH STEP == DO NOT CHANGE THIS COMMENT */
    /* END   == FINISH STEP == DO NOT CHANGE THIS COMMENT */

    return nxtSt;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of fsm-fsm.c */
