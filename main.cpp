#include "web.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

extern int    nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
extern fd_set rset, wset;
extern file   files[MAXFILES];

int main(int argc, char** argv) {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	int i, fd, n, flags, error;
	char buf[MAX_LINE];
	fd_set rs, ws;

	if (argc < 4)
	{
		printf("usage: web <hostname> <homepage> <files1> ...");
		return -1;
	}

	maxfd = -1;
	nlefttoread = nlefttoconn = nfiles = argc - 3;

	for (size_t i = 0; i < nfiles; i++)
	{
		files[i].f_flags = 0;
		files[i].f_host = argv[1];
		files[i].f_name = argv[i + 3];
	}

	home_page(argv[1], argv[2]);

	FD_ZERO(&rset);
	FD_ZERO(&wset);

	while (nlefttoread > 0)
	{
		while (nlefttoconn > 0)
		{
			for (i = 0; i < nfiles; i++)
				if (files[i].f_flags == 0)
					break;

			start_connect(&files[i]);
			nconn++;
			nlefttoconn--;
		}

		rs = rset;
		ws = wset;

		n = select(maxfd, &rs, &ws, NULL, NULL);

		for (i = 0; i < nfiles; i++)
		{
			auto cfiles = &files[i];
			flags = cfiles->f_flags;
			if (cfiles->f_flags & F_CONNECTING && (FD_ISSET(cfiles->f_fd, &rs) || FD_ISSET(cfiles->f_fd, &ws)))
			{
				n = sizeof(error);

				getsockopt(cfiles->f_fd, SOL_SOCKET, SO_ERROR, (char*)&error, &n);
				if (error != 0)
				{
					printf("\nerror during connection esteblishment %d", cfiles->f_name);
					return -1;
				}

				printf("\nconnection established for %s\n", files[i].f_name);
				FD_CLR(cfiles->f_fd, &wset); /* no more writeability test */
				write_get_cmd(&files[i]);   /* write() the GET command */
			}
			else if (flags & F_READING && FD_ISSET(cfiles->f_fd, &rs)) 
			{
				if ((n == recv(cfiles->f_fd, buf, MAX_LINE, 0)) == 0)
				{
					printf("\n%s: Data :: \n %s", files[i].f_name, buf);
					closesocket(cfiles->f_fd);
					cfiles->f_flags = F_DONE;
					FD_CLR(cfiles->f_fd, &rs);
					nlefttoread--;
					nconn--;
				}
				else
					printf("\nread %d bytes from %s\n", n, files[i].f_name);
			}
		}
	}

	return 0;
}