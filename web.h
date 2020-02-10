#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>

#define MAXFILES  20 
#define SERV      "80"
#define MAX_LINE  4096

struct file { 
	char* f_name; /* filename */ 
	char* f_host; /* hostname or IPv4/IPv6 address */ 
	int     f_fd; /* descriptor */  
	int     f_flags; /* F_xxx below */  
};

#define F_CONNECTING 1  /* connect() in progress */
#define F_READING    2  /* connect() complete; now reading */  
#define F_DONE       4  /* all done */

#define GET_CMD      "GET %s HTTP/1.0\r\n\r\n"

/* globals */ 
extern int     nconn, nfiles, nlefttoconn, nlefttoread, maxfd;  
extern fd_set  rset, wset;
extern file files[MAXFILES];

/* function prototypes */ 
SOCKET tcp_connect(const char*, const char*);
void home_page(const char*, const char*);  
void start_connect(struct file*);  
void write_get_cmd(struct file*);
