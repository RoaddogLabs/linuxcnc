/***************************************************************
* File: rem_msg.hh
* Purpose: Defines the structs passed between REMOTE clients and servers
*  as part of CMS.
* Includes:
*         1. struct REMOTE_READ_REQUEST,  REMOTE_READ_REPLY,
*       REMOTE_WRITE_REPLY, REMOTE_WRITE_REQUEST.
*       2. Function prototypes for functions that XDR encode and decode these
*       structs.
* NOTES:
*  In windows, these XDR functions need to use the Pascal calling convention
* so that the PC-NFS Toolkit DLL can call them.
*****************************************************************/

#ifndef REM_MSG_HH
#define REM_MSG_HH

class CMS_DIAGNOSTICS_INFO;
struct CMS_HEADER;

struct REMOTE_CMS_MESSAGE {
};

enum REMOTE_CMS_REQUEST_TYPE {
    NO_REMOTE_CMS_REQUEST = 0,
    REMOTE_CMS_READ_REQUEST_TYPE = 1,
    REMOTE_CMS_WRITE_REQUEST_TYPE,
    REMOTE_CMS_CHECK_IF_READ_REQUEST_TYPE,
    REMOTE_CMS_CLEAN_REQUEST_TYPE,
    REMOTE_CMS_CLEAR_REQUEST_TYPE,
    REMOTE_CMS_CLOSE_CHANNEL_REQUEST_TYPE,
    REMOTE_CMS_GET_KEYS_REQUEST_TYPE,
    REMOTE_CMS_LOGIN_REQUEST_TYPE,
    REMOTE_CMS_SET_SUBSCRIPTION_REQUEST_TYPE,
    REMOTE_CMS_READ_COMBINED_REQUEST_TYPE,
    REMOTE_CMS_BLOCKING_READ_REQUEST_TYPE,
    REMOTE_CMS_GET_BUF_NAME_REQUEST_TYPE,
    REMOTE_CMS_CANCEL_SUBSCRIPTION_REQUEST_TYPE,
    REMOTE_CMS_SET_DIAG_INFO_REQUEST_TYPE,
    REMOTE_CMS_GET_DIAG_INFO_REQUEST_TYPE,
    REMOTE_CMS_GET_MSG_COUNT_REQUEST_TYPE,
    REMOTE_CMS_GET_QUEUE_LENGTH_REQUEST_TYPE,
    REMOTE_CMS_GET_SPACE_AVAILABLE_REQUEST_TYPE,

};

struct REMOTE_CMS_REQUEST:public REMOTE_CMS_MESSAGE {
    REMOTE_CMS_REQUEST(REMOTE_CMS_REQUEST_TYPE _type) {
	type = (int) _type;
	buffer_number = 0;
	subdiv = 0;
    };
    long buffer_number;
    int type;
    int subdiv;
};

struct REMOTE_CMS_REPLY:public REMOTE_CMS_MESSAGE {
    REMOTE_CMS_REPLY() {
	status = 0;
    };
    int status;
};

/* Structure sent by client to server to initiate a read. */
struct REMOTE_BLOCKING_READ_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_BLOCKING_READ_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_BLOCKING_READ_REQUEST_TYPE) {
    };
    int access_type;		/* read or just peek */
    long last_id_read;		/* The server can compare with id from buffer 
				   to determine if the buffer is new to this
				   client */
    long timeout_millis;	/* Milliseconds for blocking_timeout or -1 to 
				   wait forever */
    void *_nml;
    void *_data;
    void *_reply;
};

/* Structure sent by client to server to initiate a read. */
struct REMOTE_GET_BUF_NAME_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_GET_BUF_NAME_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_GET_BUF_NAME_REQUEST_TYPE) {
    };
};

struct REMOTE_READ_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_READ_REQUEST():REMOTE_CMS_REQUEST(REMOTE_CMS_READ_REQUEST_TYPE) {
    };
    int access_type;		/* read or just peek */
    long last_id_read;		/* The server can compare with id from buffer 
				   to determine if the buffer is new to this
				   client */
};

/* Structure returned by server to client after a read. */
struct REMOTE_READ_REPLY:public REMOTE_CMS_REPLY {
    int size;			/* size of message stored in data. */
    long write_id;		/* Id from the buffer. */
    long was_read;		/* Was this message already read? */
    void *data;			/* Location of stored message. */
};

/* Structure returned by server to client after a read. */
struct REMOTE_GET_BUF_NAME_REPLY:public REMOTE_CMS_REPLY {
    char name[32];		/* Location of stored buffer name (truncated
				   to 31 characters). */
};

/* Structure returned by server to client after a read. */
struct REMOTE_BLOCKING_READ_REPLY:public REMOTE_READ_REPLY {
};

/* Structure sent by client to server to initiate a write. */
struct REMOTE_WRITE_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_WRITE_REQUEST():REMOTE_CMS_REQUEST(REMOTE_CMS_WRITE_REQUEST_TYPE) {
	data = NULL;
	size = 0;
    };
    int access_type;		/* write or write_if_read */
    int size;			/* size of message in data */
    void *data;			/* location of message to write into buffer */
    void *_nml;
};

/* Structure returned by server to client after a write. */
struct REMOTE_WRITE_REPLY:public REMOTE_CMS_REPLY {
    long was_read;		/* Was the message to be overwriten ever
				   read? */
    int confirm_write;
};

struct REMOTE_CHECK_IF_READ_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_CHECK_IF_READ_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_CHECK_IF_READ_REQUEST_TYPE) {
    };
};

struct REMOTE_CHECK_IF_READ_REPLY:public REMOTE_CMS_REPLY {
    int was_read;
};

struct REMOTE_CLEAR_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_CLEAR_REQUEST():REMOTE_CMS_REQUEST(REMOTE_CMS_CLEAR_REQUEST_TYPE) {
    };
};

struct REMOTE_CLEAR_REPLY:public REMOTE_CMS_REPLY {
};

struct REMOTE_CLOSE_CHANNEL_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_CLOSE_CHANNEL_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_CLOSE_CHANNEL_REQUEST_TYPE) {
    };
};

struct REMOTE_CLOSE_CHANNEL_REPLY:public REMOTE_CMS_REPLY {
};

struct REMOTE_GET_KEYS_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_GET_KEYS_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_GET_KEYS_REQUEST_TYPE) {
    };
    char name[16];
};

struct REMOTE_GET_KEYS_REPLY:public REMOTE_CMS_REPLY {
    char key1[8];
    char key2[8];
};

struct REMOTE_LOGIN_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_LOGIN_REQUEST():REMOTE_CMS_REQUEST(REMOTE_CMS_LOGIN_REQUEST_TYPE) {
    };
    char name[16];
    char passwd[16];
};

struct REMOTE_LOGIN_REPLY:public REMOTE_CMS_REPLY {
    int success;		// 1 = logged in, 0 = not
};

enum CMS_REMOTE_SUBSCRIPTION_REQUEST_TYPE {
    CMS_POLLED_SUBSCRIPTION = 1,
    CMS_NO_SUBSCRIPTION,
    CMS_VARIABLE_SUBSCRIPTION
};

struct REMOTE_SET_SUBSCRIPTION_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_SET_SUBSCRIPTION_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_SET_SUBSCRIPTION_REQUEST_TYPE) {
    };
    int subscription_type;
    int poll_interval_millis;
    int last_id_read;
};

struct REMOTE_SET_SUBSCRIPTION_REPLY:public REMOTE_CMS_REPLY {
    int success;		/* 1 = logged in, 0 = not */
    int subscription_id;	/* used by UDP clients to cancel a
				   subscription. */
};

struct REMOTE_CANCEL_SUBSCRIPTION_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_CANCEL_SUBSCRIPTION_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_CANCEL_SUBSCRIPTION_REQUEST_TYPE) {
    };
    int subscription_id;
};

struct REMOTE_CANCEL_SUBSCRIPTION_REPLY:public REMOTE_CMS_REPLY {
    int success;		/* 1 = logged in, 0 = not */
    int subscription_id;	/* used by UDP clients to cancel a
				   subscription. */
};

struct REMOTE_SET_DIAG_INFO_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_SET_DIAG_INFO_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_SET_DIAG_INFO_REQUEST_TYPE) {
    };
    char process_name[16];
    char host_sysinfo[256];
    int pid;
    int c_num;
    double rcslib_ver;
    int reverse_flag;
    double bytes_moved;
    double bytes_moved_accross_socket;
};

/* Structure returned by server to client after a read. */
struct REMOTE_SET_DIAG_INFO_REPLY:public REMOTE_CMS_REPLY {
};

struct REMOTE_GET_DIAG_INFO_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_GET_DIAG_INFO_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_GET_DIAG_INFO_REQUEST_TYPE) {
    };
};

/* Structure returned by server to client after a read. */
struct REMOTE_GET_DIAG_INFO_REPLY:public REMOTE_CMS_REPLY {
    CMS_DIAGNOSTICS_INFO *cdi;
};

struct REMOTE_GET_MSG_COUNT_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_GET_MSG_COUNT_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_GET_MSG_COUNT_REQUEST_TYPE) {
    };
};

/* Structure returned by server to client after a read. */
struct REMOTE_GET_MSG_COUNT_REPLY:public REMOTE_CMS_REPLY {
    long count;
};

struct REMOTE_GET_QUEUE_LENGTH_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_GET_QUEUE_LENGTH_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_GET_QUEUE_LENGTH_REQUEST_TYPE) {
    };
};

/* Structure returned by server to client after a read. */
struct REMOTE_GET_QUEUE_LENGTH_REPLY:public REMOTE_CMS_REPLY {
    long queue_length;
};

struct REMOTE_GET_SPACE_AVAILABLE_REQUEST:public REMOTE_CMS_REQUEST {
    REMOTE_GET_SPACE_AVAILABLE_REQUEST():REMOTE_CMS_REQUEST
	(REMOTE_CMS_GET_SPACE_AVAILABLE_REQUEST_TYPE) {
    };
};

/* Structure returned by server to client after a read. */
struct REMOTE_GET_SPACE_AVAILABLE_REPLY:public REMOTE_CMS_REPLY {
    long space_available;
};

#endif
