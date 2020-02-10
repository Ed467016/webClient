#include "web.h"

extern int nconn = 0, nfiles = 0, nlefttoconn = 0, nlefttoread = 0, maxfd = -1;
extern fd_set rset = fd_set(), wset = fd_set();
extern file files[MAXFILES] = {};

void home_page(const char* host, const char* fname) {
	SOCKET connfd;
	int n;
	char buff[MAX_LINE];
	
	snprintf(buff, MAX_LINE, GET_CMD, fname);

	connfd = tcp_connect(host, SERV);
	send(connfd, buff, MAX_LINE, 0);

	for (;;)
	{
		if ((n = recv(connfd, buff, MAX_LINE, 0)) == 0)
			break;

		printf("read %d bytes of home page\n", n);
	}

	printf("finished recieving home page \n");
	printf(buff);
	closesocket(connfd);
}