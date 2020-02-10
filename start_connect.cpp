#include "web.h"

extern int     nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
extern fd_set  rset, wset;

void start_connect(struct file* file) {
	SOCKET sockfd;
	int n, flags;
	char buff[MAX_LINE];
	addrinfo hints, * padr;
	unsigned long ul = 1;

	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_CANONNAME;

	getaddrinfo(file->f_host, SERV, &hints, &padr);
	sockfd = socket(padr->ai_family, padr->ai_socktype, padr->ai_protocol);
	file->f_fd = sockfd;
	ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);

	if ((n = connect(sockfd, padr->ai_addr, padr->ai_addrlen)) < 0)
	{
		file->f_flags = F_CONNECTING;
		FD_SET(sockfd, &rset);
		FD_SET(sockfd, &wset);

		if (sockfd > maxfd)
		{
			maxfd = sockfd;
		}
	}
	else if (n >= 0)
	{
		write_get_cmd(file);
	}
}