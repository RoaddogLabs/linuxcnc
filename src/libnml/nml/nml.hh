/*************************************************************************
* File: nml.hh                                                           *
* Authors: Fred Proctor, Will Shackleford                                *
* Purpose: C++ Header file for the Neutral Manufacturing Language (NML). *
*          Includes:                                                     *
*               1. class NML.                                            *
*               2. typedef for NMLTYPE.                                  *
*               3. typedef for NML_FORMAT_PTR                            *
*               4. enum NML_ERROR_TYPE                                   *
*************************************************************************/

#ifndef NML_HH
#define NML_HH

/* Include Files */

extern "C" {
#include <stddef.h>		/* size_t */
}
#include "cms_user.hh"		/* class CMS_USER */
#include "linklist.hh"		/* class LinkedList */
#include "nmlmsg.hh"
/* Generic NML Stuff */
#ifndef NMLTYPE_TYPEDEFED
#define NMLTYPE_TYPEDEFED
typedef long NMLTYPE;		/* Also defined in nmlmsg.hh */
#endif

//class NMLmsg;                 /* Use only partial definition to avoid
//                                 depending on nmlmsg.hh. */

/* Typedef for pointer to the function used to decode a message by its id number. */
typedef int (*NML_FORMAT_PTR) (NMLTYPE, void *, CMS *);

/* Values for NML::error_type. */
enum NML_ERROR_TYPE {
    NML_NO_ERROR,
    NML_BUFFER_NOT_READ,
    NML_TIMED_OUT,
    NML_INVALID_CONFIGURATION,
    NML_FORMAT_ERROR,
    NML_INTERNAL_CMS_ERROR,
    NML_NO_MASTER_ERROR,
    NML_INVALID_MESSAGE_ERROR,
    NML_QUEUE_FULL_ERROR
};

enum NML_CHANNEL_TYPE {
    INVALID_NML_CHANNEL_TYPE = 0,
    NML_GENERIC_CHANNEL_TYPE,
    RCS_CMD_CHANNEL_TYPE,
    RCS_STAT_CHANNEL_TYPE,
    NML_QUERY_CHANNEL_TYPE,
    NML_ID_CHANNEL_TYPE
};

extern char NML_ERROR_TYPE_STRINGS[8][80];
class NML_DIAGNOSTICS_INFO;

/* nml interface to CMS. */
class NML:public virtual CMS_USER {
  protected:
    int run_format_chain(NMLTYPE, void *);
    int format_input(NMLmsg * nml_msg);	/* Format message if neccessary */
    int format_output();	/* Decode message if neccessary. */

  public:
    void *operator                          new(size_t);
    void operator                          delete(void *);
    LinkedList *format_chain;
    void register_with_server();	/* Add this channel to the server's
					   list. */
    void add_to_channel_list();	/* Add this channel to the main list.  */
    int channel_list_id;	/* List id of this channel. */
    NML_ERROR_TYPE error_type;	/* check here if an NML function returns -1 */

    /* Get Address of message for user after read. */
      NMLTYPE(*phantom_read) ();
      NMLTYPE(*phantom_peek) ();
    int (*phantom_write) (NMLmsg * nml_msg);
    int (*phantom_write_if_read) (NMLmsg * nml_msg);
    int (*phantom_check_if_read) ();
    int (*phantom_clear) ();
    int ignore_format_chain;

    NMLmsg *get_address();
    void delete_channel();

    /* Read and Write Functions. */
    NMLTYPE read();		/* Read the buffer. */
    NMLTYPE blocking_read(double timeout);	/* Read the buffer. (Wait for 
						   new data). */
    NMLTYPE peek();		/* Read buffer without changing was_read */
    NMLTYPE read(void *temp_data, long temp_size);
    NMLTYPE peek(void *temp_data, long temp_size);
    int write(NMLmsg & nml_msg);	/* Write a message. (Use reference) */
    int write(NMLmsg * nml_msg);	/* Write a message. (Use pointer) */
    int write_if_read(NMLmsg & nml_msg);	/* Write only if buffer
						   was_read */
    int write_if_read(NMLmsg * nml_msg);	/* '' */

    const char *msg2str(NMLmsg & nml_msg);
    const char *msg2str(NMLmsg * nml_msg);

    /* Function to check to see if this NML object is properly configured. */
    int valid();

    /* Get Diagnostics Information. */
    NML_DIAGNOSTICS_INFO *get_diagnostics_info();

    int prefix_format_chain(NML_FORMAT_PTR);

    /* Constructors and destructors. */
      NML(NML_FORMAT_PTR f_ptr,
	char *, char *, char *, int set_to_server = 0, int set_to_master = 0);
      NML(NML *, int set_to_server = 0, int set_to_master = 0);
      NML(char *buffer_line, char *proc_line);
      virtual ~ NML();
    int reset();

    int set_error();
    void print_info(char *bufname = NULL, char *procname =
	NULL, char *cfg_file = NULL);
  protected:

    int fast_mode;
    int *cms_status;
    long *cms_inbuffer_header_size;
      NML(char *, char *, char *, int set_to_server = 0, int set_to_master =
	0);
    void reconstruct(NML_FORMAT_PTR, char *, char *, char *,
	int set_to_server = 0, int set_to_master = 0);

    int info_printed;

  public:
      NML_CHANNEL_TYPE channel_type;
    long sizeof_message_header;
    int forced_type;

  protected:
    int already_deleted;
    char bufname[40];
    char procname[40];
    char cfgfilename[160];
    double blocking_read_poll_interval;
    CMS *cms_for_msg_string_conversions;

      NML(NML & nml);		// Don't copy me.
};

int create_NML(NML **, NML_FORMAT_PTR f_ptr,
    char *buf, char *proc, char *file);

void free_NML(NML *);

extern LinkedList *NML_Main_Channel_List;
extern "C" {
    extern void nml_start();
    extern void nml_cleanup();
    extern void nml_wipeout_lists();
    extern void set_default_nml_config_file(const char *);
    extern const char *get_default_nml_config_file();

    extern void nmlSetHostAlias(const char *hostName, const char *hostAlias);
    extern void nmlClearHostAliases();
    extern void nmlAllowNormalConnection();
    extern void nmlForceRemoteConnection();
    extern void nmlForceLocalConnection();
} extern int verbose_nml_error_messages;
extern int nml_print_hostname_on_error;
extern int nml_reset_errors_printed;

#endif /* !NML_HH */
