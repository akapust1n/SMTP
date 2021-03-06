/*   -*- buffer-read-only: t -*- vi: set ro:
 *
 *  DO NOT EDIT THIS FILE   (checkoptn.h)
 *
 *  It has been AutoGen-ed
 *  From the definitions    checkoptn.def
 *  and the template file   options
 *
 * Generated from AutoOpts 41:1:16 templates.
 *
 *  AutoOpts is a copyrighted work.  This header file is not encumbered
 *  by AutoOpts licensing, but is provided under the licensing terms chosen
 *  by the smtp_client author or copyright holder.  AutoOpts is
 *  licensed under the terms of the LGPL.  The redistributable library
 *  (``libopts'') is licensed under the terms of either the LGPL or, at the
 *  users discretion, the BSD license.  See the AutoOpts and/or libopts sources
 *  for details.
 */
/**
 *  This file contains the programmatic interface to the Automated
 *  Options generated for the smtp_client program.
 *  These macros are documented in the AutoGen info file in the
 *  "AutoOpts" chapter.  Please refer to that doc for usage help.
 */
#ifndef AUTOOPTS_CHECKOPTN_H_GUARD
#define AUTOOPTS_CHECKOPTN_H_GUARD 1
#include <autoopts/options.h>

/**
 *  Ensure that the library used for compiling this generated header is at
 *  least as new as the version current when the header template was released
 *  (not counting patch version increments).  Also ensure that the oldest
 *  tolerable version is at least as old as what was current when the header
 *  template was released.
 */
#define AO_TEMPLATE_VERSION 167937
#if (AO_TEMPLATE_VERSION < OPTIONS_MINIMUM_VERSION) \
 || (AO_TEMPLATE_VERSION > OPTIONS_STRUCT_VERSION)
# error option template version mismatches autoopts/options.h header
  Choke Me.
#endif

/**
 *  Enumeration of each option type for smtp_client
 */
typedef enum {
    INDEX_OPT_ROOT_DIR                        =  0,
    INDEX_OPT_LOG_FILE_NAME                   =  1,
    INDEX_OPT_MAIL_SEND_TIMEOUT               =  2,
    INDEX_OPT_MAIL_RETRY_WAIT_TIME            =  3,
    INDEX_OPT_NUMBER_OF_WORKERS               =  4,
    INDEX_OPT_RANDOM_DIRECTORIES_FOR_WORKERS  =  5,
    INDEX_OPT_OUTGOING_MAIL_BASE_DIR          =  6,
    INDEX_OPT_OUTGOING_MAIL_PROCESS_DIR       =  7,
    INDEX_OPT_OUTGOING_MAIL_SENT_DIR          =  8,
    INDEX_OPT_HELP                            =  9,
    INDEX_OPT_MORE_HELP                       = 10
} teOptIndex;
/** count of all options for smtp_client */
#define OPTION_CT    11

/**
 *  Interface defines for all options.  Replace "n" with the UPPER_CASED
 *  option name (as in the teOptIndex enumeration above).
 *  e.g. HAVE_OPT(ROOT_DIR)
 */
#define         DESC(n) (smtp_clientOptions.pOptDesc[INDEX_OPT_## n])
/** 'true' if an option has been specified in any way */
#define     HAVE_OPT(n) (! UNUSED_OPT(& DESC(n)))
/** The string argument to an option. The argument type must be \"string\". */
#define      OPT_ARG(n) (DESC(n).optArg.argString)
/** Mask the option state revealing how an option was specified.
 *  It will be one and only one of \a OPTST_SET, \a OPTST_PRESET,
 * \a OPTST_DEFINED, \a OPTST_RESET or zero.
 */
#define    STATE_OPT(n) (DESC(n).fOptState & OPTST_SET_MASK)
/** Count of option's occurrances *on the command line*. */
#define    COUNT_OPT(n) (DESC(n).optOccCt)
/** mask of \a OPTST_SET and \a OPTST_DEFINED. */
#define    ISSEL_OPT(n) (SELECTED_OPT(&DESC(n)))
/** 'true' if \a HAVE_OPT would yield 'false'. */
#define ISUNUSED_OPT(n) (UNUSED_OPT(& DESC(n)))
/** 'true' if OPTST_DISABLED bit not set. */
#define  ENABLED_OPT(n) (! DISABLED_OPT(& DESC(n)))
/** number of stacked option arguments.
 *  Valid only for stacked option arguments. */
#define  STACKCT_OPT(n) (((tArgList*)(DESC(n).optCookie))->useCt)
/** stacked argument vector.
 *  Valid only for stacked option arguments. */
#define STACKLST_OPT(n) (((tArgList*)(DESC(n).optCookie))->apzArgs)
/** Reset an option. */
#define    CLEAR_OPT(n) STMTS( \
                DESC(n).fOptState &= OPTST_PERSISTENT_MASK;   \
                if ((DESC(n).fOptState & OPTST_INITENABLED) == 0) \
                    DESC(n).fOptState |= OPTST_DISABLED; \
                DESC(n).optCookie = NULL )
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *  Enumeration of smtp_client exit codes
 */
typedef enum {
    SMTP_CLIENT_EXIT_SUCCESS         = 0,
    SMTP_CLIENT_EXIT_FAILURE         = 1,
    SMTP_CLIENT_EXIT_USAGE_ERROR     = 64,
    SMTP_CLIENT_EXIT_LIBOPTS_FAILURE = 70
}   smtp_client_exit_code_t;
/**
 *  Interface defines for specific options.
 * @{
 */
#define VALUE_OPT_ROOT_DIR       0x1001
#define VALUE_OPT_LOG_FILE_NAME  0x1002
#define VALUE_OPT_MAIL_SEND_TIMEOUT 0x1003

#define OPT_VALUE_MAIL_SEND_TIMEOUT (DESC(MAIL_SEND_TIMEOUT).optArg.argInt)
#define VALUE_OPT_MAIL_RETRY_WAIT_TIME 0x1004

#define OPT_VALUE_MAIL_RETRY_WAIT_TIME (DESC(MAIL_RETRY_WAIT_TIME).optArg.argInt)
#define VALUE_OPT_NUMBER_OF_WORKERS 'n'

#define OPT_VALUE_NUMBER_OF_WORKERS (DESC(NUMBER_OF_WORKERS).optArg.argInt)
#define VALUE_OPT_RANDOM_DIRECTORIES_FOR_WORKERS 0x1005
#define VALUE_OPT_OUTGOING_MAIL_BASE_DIR 0x1006
#define VALUE_OPT_OUTGOING_MAIL_PROCESS_DIR 0x1007
#define VALUE_OPT_OUTGOING_MAIL_SENT_DIR 0x1008
/** option flag (value) for help-value option */
#define VALUE_OPT_HELP          '?'
/** option flag (value) for more-help-value option */
#define VALUE_OPT_MORE_HELP     '!'
/*
 *  Interface defines not associated with particular options
 */
#define ERRSKIP_OPTERR  STMTS(smtp_clientOptions.fOptSet &= ~OPTPROC_ERRSTOP)
#define ERRSTOP_OPTERR  STMTS(smtp_clientOptions.fOptSet |= OPTPROC_ERRSTOP)
#define RESTART_OPT(n)  STMTS( \
                smtp_clientOptions.curOptIdx = (n); \
                smtp_clientOptions.pzCurOpt  = NULL )
#define START_OPT       RESTART_OPT(1)
#define USAGE(c)        (*smtp_clientOptions.pUsageProc)(&smtp_clientOptions, c)

#ifdef  __cplusplus
extern "C" {
#endif


/* * * * * *
 *
 *  Declare the smtp_client option descriptor.
 */
extern tOptions smtp_clientOptions;

#if defined(ENABLE_NLS)
# ifndef _
#   include <stdio.h>
#   ifndef HAVE_GETTEXT
      extern char * gettext(char const *);
#   else
#     include <libintl.h>
#   endif

# ifndef ATTRIBUTE_FORMAT_ARG
#   define ATTRIBUTE_FORMAT_ARG(_a)
# endif

static inline char* aoGetsText(char const* pz) ATTRIBUTE_FORMAT_ARG(1);
static inline char* aoGetsText(char const* pz) {
    if (pz == NULL) return NULL;
    return (char*)gettext(pz);
}
#   define _(s)  aoGetsText(s)
# endif /* _() */

# define OPT_NO_XLAT_CFG_NAMES  STMTS(smtp_clientOptions.fOptSet |= \
                                    OPTPROC_NXLAT_OPT_CFG;)
# define OPT_NO_XLAT_OPT_NAMES  STMTS(smtp_clientOptions.fOptSet |= \
                                    OPTPROC_NXLAT_OPT|OPTPROC_NXLAT_OPT_CFG;)

# define OPT_XLAT_CFG_NAMES     STMTS(smtp_clientOptions.fOptSet &= \
                                  ~(OPTPROC_NXLAT_OPT|OPTPROC_NXLAT_OPT_CFG);)
# define OPT_XLAT_OPT_NAMES     STMTS(smtp_clientOptions.fOptSet &= \
                                  ~OPTPROC_NXLAT_OPT;)

#else   /* ENABLE_NLS */
# define OPT_NO_XLAT_CFG_NAMES
# define OPT_NO_XLAT_OPT_NAMES

# define OPT_XLAT_CFG_NAMES
# define OPT_XLAT_OPT_NAMES

# ifndef _
#   define _(_s)  _s
# endif
#endif  /* ENABLE_NLS */

#ifdef  __cplusplus
}
#endif
#endif /* AUTOOPTS_CHECKOPTN_H_GUARD */

/* checkoptn.h ends here */
