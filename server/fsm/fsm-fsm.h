/*   -*- buffer-read-only: t -*- vi: set ro:
 *
 *  DO NOT EDIT THIS FILE   (fsm-fsm.h)
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
/*
 *  This file enumerates the states and transition events for a FSM.
 *
 *  te_ss_state
 *      The available states.  FSS_INIT is always defined to be zero
 *      and FSS_INVALID and FSS_DONE are always made the last entries.
 *
 *  te_ss_event
 *      The transition events.  These enumerate the event values used
 *      to select the next state from the current state.
 *      SS_EV_INVALID is always defined at the end.
 */
#ifndef AUTOFSM_FSM_FSM_H_GUARD
#define AUTOFSM_FSM_FSM_H_GUARD 1
/**
 *  Finite State machine States
 *
 *  Count of non-terminal states.  The generated states INVALID and DONE
 *  are terminal, but INIT is not  :-).
 */
#define SS_STATE_CT  9
typedef enum {
    SS_ST_INIT,    SS_ST_HELO,    SS_ST_MAIL,    SS_ST_RCPT,    SS_ST_DATA,
    SS_ST_GMSG,    SS_ST_TMSG,    SS_ST_READY,   SS_ST_ERROR,   SS_ST_INVALID,
    SS_ST_DONE
} te_ss_state;

/**
 *  Finite State machine transition Events.
 *
 *  Count of the valid transition events
 */
#define SS_EVENT_CT 12
typedef enum {
    SS_EV_EHLO,    SS_EV_HELO,    SS_EV_MAIL,    SS_EV_RCPT,    SS_EV_DATA,
    SS_EV_RSET,    SS_EV_VRFY,    SS_EV_NOP_,    SS_EV_QUIT,    SS_EV_TMSG,
    SS_EV_GMSG,    SS_EV_ERROR,   SS_EV_INVALID
} te_ss_event;

/**
 *  Step the FSM.  Returns the resulting state.  If the current state is
 *  SS_ST_DONE or SS_ST_INVALID, it resets to
 *  SS_ST_INIT and returns SS_ST_INIT.
 */
extern te_ss_state
ss_step(
    te_ss_state ss_state,
    te_ss_event trans_evt,
    int fd, char** matchdata, int matchdatal, char* recv_buf, int recv_sz );

#endif /* AUTOFSM_FSM_FSM_H_GUARD */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of fsm-fsm.h */
